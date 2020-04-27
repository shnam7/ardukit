/**
 *  @package Ardukit
 *
 *  @module list - doubly linked list
 */

#include "glist.h"

unsigned gcl::_node::length()
{
    unsigned count = 0;
    _node *nod = m_next;
    while (nod->m_next != m_next) {
        nod = nod->m_next;
        count++;
    }
    return count;
}
