#ifndef _IDOCEXPORTER_H_
#define _IDOCEXPORTER_H_

#pragma warning (disable: 4250) // inheritance via dominance

#include "EDocExporter.h"
#include "IOutputFileName.h"
#include <ostream>

//! @brief Common interface for all document exporters
class IDocExporter : public virtual IOutputFileName
{
private:
    IDocExporter(const IDocExporter&) = delete;
    IDocExporter operator=(const IDocExporter&) = delete;

public:
    IDocExporter() = default;
    virtual ~IDocExporter() = default;

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