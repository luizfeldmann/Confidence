/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

/**
 * @file IDocExporter.h
 * @brief Defines interface #IDocExporter
 */

#ifndef _IDOCEXPORTER_H_
#define _IDOCEXPORTER_H_

#pragma warning (disable: 4250) // inheritance via dominance

#include "EDocExporter.h"
#include "IDocumentationContext.h"
#include "IOutputFileName.h"
#include <ostream>

//! @brief Common interface for all document exporters
class IDocExporter : public IDocumentationContext, public virtual IOutputFileName
{
private:
    IDocExporter(const IDocExporter&) = delete;
    IDocExporter operator=(const IDocExporter&) = delete;

public:
    IDocExporter() = default;
    virtual ~IDocExporter() = default;

    /* Heading */

    //! @brief Gets the current heading level (title, subtitle, section ...)
    virtual int GetHeading() const = 0;

    //! @brief Increments the heading (more nested)
    virtual void IncrementHeading() = 0;

    //! @brief Decrements the heading (less nested)
    virtual void DecrementHeading() = 0;

    //! @brief Starts a heading and pushes to the stack
    virtual bool Heading() = 0;

    //! @brief Starts a container and pushes to the stack
    virtual bool Container(const std::string& strClass, const std::string& strId) = 0;

    //! @brief Pops the last node from the stack
    virtual bool PopStack() = 0;

    /* Style */

    //! @brief Pushes the "strong" style to the stack
    virtual bool Strong() = 0;

    //! @brief Pushes the "emphasis" style to the stack
    virtual bool Emphasis() = 0;

    /* Content */

    //! @brief Starts a new paragraph and pushes to the stack
    virtual bool Paragraph() = 0;

    //! @brief Appends text to the current paragraph in the stack
    virtual bool Text(const std::string&) = 0;

    //! @brief Appends a text in format 'Label: Value', where the Label is styled with emphasis
    virtual bool FormField(const std::string& strLabel, const std::string& strValue, bool bCode = false) = 0;

    //! @brief Starts a new list and pushes it in the stack
    virtual bool List() = 0;

    //! @brief Appends a new item and pushes it in the stack
    virtual bool Item() = 0;

    //! @brief Appends a code block to the document tree and pushes it to the stack
    virtual bool Code(const std::string& strText) = 0;

    //! @brief Appends a block of raw HTML code
    virtual bool Html(const std::string& strHtml) = 0;

    //! @brief Appends a collapsible section and pushes it to the stack
    virtual bool Collapsible() = 0;

    //! @brief Appends a summary tag inside a collapsible
    virtual bool Summary() = 0;

    /* Control */

    //! @brief Starts the exporter to the provided stream
    //! @return True if success
    virtual bool Start(std::ostream& rOutput) = 0;

    //! @brief Ends the exporter session and releases the stream
    //! @return True if success
    virtual bool Finish() = 0;

    //! @brief Reflects the type of document exporter
    virtual EDocExporter GetType() const = 0;
};

#endif