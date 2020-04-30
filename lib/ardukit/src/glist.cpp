/**
 *  @package Ardukit
 *
 *  @module list - doubly linked list
 */

#include "glist.h"

gcl_api unsigned gcl::glist::node::length()
{
    unsigned count = 0;
    gcl::glist::node *nod = m_next;
    while (nod->m_next != m_next) {
        nod = nod->m_next;
        count++;
    }
    return count;
}
