#ifndef MCP42XXX_H
#define MCP42XXX_H

typedef uint8 MCP42xxx_device_handle;

void MCP42xxxInit(); // Call this function first
void MCP42xxx_setResistance(uint8 potnum, uint8 resistance, MCP42xxx_device_handle device);

void Select(struct MCP42xxx_device_handle device);
void Deselect(struct MCP42xxx_device_handle device);

#endif

