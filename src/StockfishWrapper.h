#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <stdexcept>
#include <chrono>
#include <algorithm>
#include <locale>
#include <codecvt>
#include <sstream>

// Utility function to convert std::string to std::wstring
// std::wstring string2W(const std::string& str) {
//     std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
//     return converter.from_bytes(str);
// }

// std::string wstringToString(const std::wstring& wstr) {
//     std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
//     return converter.to_bytes(wstr);
// }

class StockfishWrapper {
public:
    StockfishWrapper(const std::wstring &path) : m_path(path) {
        std::wcout << L"Starting Stockfish process at: " << path << std::endl;

        SECURITY_ATTRIBUTES saAttr;
        saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
        saAttr.bInheritHandle = TRUE;
        saAttr.lpSecurityDescriptor = NULL;

        if (!CreatePipe(&m_hChildStdoutRead, &m_hChildStdoutWrite, &saAttr, 0) ||
            !CreatePipe(&m_hChildStdinRead, &m_hChildStdinWrite, &saAttr, 0)) {
            throw std::runtime_error("Failed to create pipes");
        }

        if (!SetHandleInformation(m_hChildStdoutRead, HANDLE_FLAG_INHERIT, 0))
            throw std::runtime_error("Failed to set pipe information");

        STARTUPINFOW siStartInfo;
        ZeroMemory(&siStartInfo, sizeof(STARTUPINFOW));
        siStartInfo.cb = sizeof(STARTUPINFOW);
        siStartInfo.hStdError = m_hChildStdoutWrite;
        siStartInfo.hStdOutput = m_hChildStdoutWrite;
        siStartInfo.hStdInput = m_hChildStdinRead;
        siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

        if (!CreateProcessW(
            NULL,
            const_cast<LPWSTR>(m_path.c_str()),
            NULL,
            NULL,
            TRUE,
            0,
            NULL,
            NULL,
            &siStartInfo,
            &m_piProcInfo)) {
            throw std::runtime_error("Failed to start Stockfish. Error code: " + std::to_string(GetLastError()));
        }

        std::wcout << L"Stockfish process started successfully" << std::endl;
        std::wcout << L"Sending UCI command..." << std::endl;
        std::string initialOutput = sendCommand("uci", "uciok", 5000);
        // std::wcout << L"Initial output: " << string2W(initialOutput) << std::endl;

        // Ensure the engine is ready
        sendCommand("isready", "readyok", 5000);
    }

    ~StockfishWrapper() {
        if (m_piProcInfo.hProcess != NULL) {
            TerminateProcess(m_piProcInfo.hProcess, 0);
            CloseHandle(m_piProcInfo.hProcess);
            CloseHandle(m_piProcInfo.hThread);
        }
        CloseHandle(m_hChildStdinRead);
        CloseHandle(m_hChildStdinWrite);
        CloseHandle(m_hChildStdoutRead);
        CloseHandle(m_hChildStdoutWrite);
    }

    std::string sendCommand(const std::string &command, const std::string &expectedResponse = "", int timeoutMs = 1000) {
        DWORD bytesWritten;
        std::string cmd = command + "\n";
        std::cout << "Sending command: " << cmd;
        if (!WriteFile(m_hChildStdinWrite, cmd.c_str(), cmd.length(), &bytesWritten, NULL)) {
            throw std::runtime_error("Failed to write to pipe");
        }
        std::cout << "Command sent. Reading output..." << std::endl;
        return readOutput(expectedResponse, timeoutMs);
    }

    std::string getBestMoveTimed(const std::string &fen, int analysisTimeMs = 1000) {
        sendCommand("position fen " + fen);
        std::string output = sendCommand("go movetime " + std::to_string(analysisTimeMs), "bestmove", analysisTimeMs + 500);
        return parseBestMove(output);
    }

    std::string getBestMoveAtDepth(const std::string &fen, int depth) {
        sendCommand("position fen " + fen);
        std::string output = sendCommand("go depth " + std::to_string(depth), "bestmove", 1000);
        return parseBestMove(output);
    }

    std::pair<std::string, int> getBestMoveWithDepth(const std::string &fen, int analysisTimeMs = 1000) {
        sendCommand("position fen " + fen);
        std::string output = sendCommand("go movetime " + std::to_string(analysisTimeMs), "bestmove", analysisTimeMs + 500);
        int maxDepth = parseMaxDepth(output);
        std::string bestMove = parseBestMove(output);
        return std::make_pair(bestMove, maxDepth);
    }

private:
    std::wstring m_path;
    HANDLE m_hChildStdinRead = NULL;
    HANDLE m_hChildStdinWrite = NULL;
    HANDLE m_hChildStdoutRead = NULL;
    HANDLE m_hChildStdoutWrite = NULL;
    PROCESS_INFORMATION m_piProcInfo;

    std::string readOutput(const std::string &expectedResponse, int timeoutMs) {
        DWORD dwRead;
        CHAR chBuf[4096];
        std::string output;
        BOOL bSuccess = FALSE;
        auto start = std::chrono::steady_clock::now();

        while (true) {
            DWORD dwAvail = 0;
            if (!PeekNamedPipe(m_hChildStdoutRead, NULL, 0, NULL, &dwAvail, NULL)) {
                throw std::runtime_error("Failed to peek pipe");
            }

            if (dwAvail > 0) {
                bSuccess = ReadFile(m_hChildStdoutRead, chBuf, std::min(dwAvail, (DWORD)4096), &dwRead, NULL);
                if (!bSuccess || dwRead == 0) break;
                output.append(chBuf, dwRead);
                if (!expectedResponse.empty() && output.find(expectedResponse) != std::string::npos) {
                    std::cout << "Expected response received: " << expectedResponse << std::endl;
                    break;
                }
            } else if (expectedResponse.empty()) {
                // If no expected response and no data available, return immediately
                return output;
            }

            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
            if (elapsed > timeoutMs) {
                std::cout << "Timeout reached. Partial output: " << output << std::endl;
                throw std::runtime_error("Timeout while reading output");
            }

            // Sleep(10); // Small delay to prevent busy waiting
        }
        return output;
    }

    std::string parseBestMove(const std::string &output) {
        std::string::size_type pos = output.find("bestmove ");
        if (pos == std::string::npos) return "";
        pos += 9; // Length of "bestmove "
        std::string::size_type end = output.find(' ', pos);
        return output.substr(pos, end - pos);
    }

    int parseMaxDepth(const std::string &output) {
        int maxDepth = 0;
        std::istringstream iss(output);
        std::string line;
        while (std::getline(iss, line)) {
            if (line.substr(0, 5) == "info ") {
                std::istringstream lineStream(line);
                std::string token;
                while (lineStream >> token) {
                    if (token == "depth") {
                        int depth;
                        if (lineStream >> depth) {
                            maxDepth = std::max(maxDepth, depth);
                        }
                        break;
                    }
                }
            }
        }
        return maxDepth;
    }
};


