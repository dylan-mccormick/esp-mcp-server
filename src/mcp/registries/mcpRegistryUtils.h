// mcpRegistryUtils.h
// Defines a few utility macro definitions to help name static variables

#pragma once

#define MCP_HANDLER_CONCAT_(a, b) a##b
#define MCP_HANDLER_CONCAT(a, b) MCP_HANDLER_CONCAT_(a, b)