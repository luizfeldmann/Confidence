#ifndef _CMARK_NODE_WRAPPER_H_
#define _CMARK_NODE_WRAPPER_H_

#include <cmark.h>

//! @brief Utility to contain and automatically free a node
class CMarkNodeWrapper
{
private:
    CMarkNodeWrapper(const CMarkNodeWrapper&) = delete;
    CMarkNodeWrapper& operator=(const CMarkNodeWrapper&) = delete;

protected:
    bool m_bOwned;              //!< If true, the wrapper owns the pointer and deletes it when destroyed
    cmark_node* m_pNode;        //!< Pointer to the wrapped node

public:
    //! @brief Instantiates a new node of the give type
    //! @param[in] eType The type of node to create and wrap
    CMarkNodeWrapper(cmark_node_type eType);

    //! @brief Frees the wrapped node *if* it's still owned
    ~CMarkNodeWrapper();

    //! @brief Frees the wrapped node *if* it's still owned
    void reset();

    //! @brief The pointer ownership was given away,
    //!        we should no longer free the node
    void disown();

    //! @brief Implicitly convert the object to the wrapped pointer
    operator cmark_node* ();

    //! @brief Checks if the wrapped node exists
    operator bool() const;
};

#endif