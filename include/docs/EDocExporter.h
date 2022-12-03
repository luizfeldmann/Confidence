/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

/**
 * @file EDocExporter.h
 * @brief Defines enum #EDocExporter
 */

#ifndef _EDOCEXPORTER_H_
#define _EDOCEXPORTER_H_

//! @brief Enumeration of classes of project exporter
enum class EDocExporter
{
    Markdown,   //!< Exports the project documentation as Markdown
    HTML,       //!< Exports the project documentation as HTML
    LaTeX,      //!< Exports the project documentation as LaTeX
};

#endif