/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#include "docs/CMarkNodeWrapper.h"

CMarkNodeWrapper::CMarkNodeWrapper(cmark_node_type eType)
    : m_bOwned(true)
    , m_pNode(cmark_node_new(eType))
{

}

CMarkNodeWrapper::~CMarkNodeWrapper()
{
    reset();
}

void CMarkNodeWrapper::reset()
{
    if (m_bOwned && m_pNode)
    {
        cmark_node_free(m_pNode);
        m_pNode = nullptr;
        m_bOwned = false;
    }
}

void CMarkNodeWrapper::disown()
{
    m_bOwned = false;
}

CMarkNodeWrapper::operator cmark_node* ()
{
    return m_pNode;
}

CMarkNodeWrapper::operator bool() const
{
    return (nullptr != m_pNode);
}