/**
 *  @package Ardukit
 *
 *  @module list - doubly linked list
 */

#include "glist.h"

unsigned adk::glist::node::length()
{
    unsigned count = 0;
    adk::glist::node *nod = m_next;
    while (nod->m_next != m_next) {
        nod = nod->m_next;
        count++;
    }
    return count;
}
