// mcpConfig.h

#pragma once

namespace McpServerInfo {
// Basic MCP Server meta
constexpr const char* name = "ESP32-MCP-Server";
constexpr const char* version = "0.1.0";
constexpr const char* description =
    "MCP server running on an ESP32 intended to allow pin I/O and library usage from LLMs.";
constexpr const char* supportedProtocols[] = {"2026-07-28"};

constexpr const char* instructions = nullptr;

// Cache info
constexpr const int ttlMs = 3600000;
}  // namespace McpServerInfo

namespace McpServerCapabilities {
// Prompts
constexpr bool prompts = false;
constexpr bool promptsListChanged = false;

// Resources
constexpr bool resources = true;
constexpr bool resourcesListChanged = false;
constexpr bool resourcesSubscribe = false;

// Tools
constexpr bool tools = true;
constexpr bool toolsListChanged = false;
}  // namespace McpServerCapabilities