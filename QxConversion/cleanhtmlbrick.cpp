#include "cleanhtmlbrick.h"
#include <QDebug>
#include <QRegularExpression>

CleanHtmlBrick::CleanHtmlBrick(QObject *parent) : QObject(parent) {
    css = "body { font-family: Arial, sans-serif; margin: 10mm; padding: 0; line-height: 1.4; }\n"
          "p { margin: 0 0 14pt 0; }\n"
          "p:empty { display: none; }\n"
          "strong { font-weight: bold; }\n"
          "em { font-style: italic; }\n"
          "table { border-collapse: collapse; width: auto; margin-top: 14pt; }\n"
          "td, th { border: 1px solid black; padding: 5px; text-align: left; }\n";
    qDebug() << "CleanHtmlBrick initialized";
}

QString CleanHtmlBrick::cleanHtml(const QString &html) {
    QString cleaned = html;
    // Strip Qt noise, preserve content
    cleaned.replace(QRegularExpression("<h[1-6][^>]*>.*?</h[1-6]>"), "");
    cleaned.replace(QRegularExpression("<b\\b[^>]*>"), "<strong>");
    cleaned.replace(QRegularExpression("</b>"), "</strong>");
    cleaned.replace(QRegularExpression("<i\\b[^>]*>"), "<em>");
    cleaned.replace(QRegularExpression("</i>"), "</em>");
    cleaned.replace(QRegularExpression("<span\\b[^>]*>(.*?)</span>"), "\\1");
    cleaned.replace(QRegularExpression("data-[^=]+=\"[^\"]+\""), "");
    cleaned.replace(QRegularExpression("style=\"[^\"]*\""), "");
    cleaned.replace(QRegularExpression("<title>.*</title>"), "");
    // Table cleanup
    cleaned.replace(QRegularExpression("<table\\s*[^>]*>"), "<table border=\"1\">");
    cleaned.replace(QRegularExpression("<td\\s*[^>]*>"), "<td>");
    cleaned.replace(QRegularExpression("<tr\\s*[^>]*>"), "<tr>");
    // Paragraph cleanup
    cleaned.replace(QRegularExpression("<p\\s*[^>]*></p>"), "");
    cleaned.replace(QRegularExpression("<p\\s*[^>]*>\\s*</p>"), "");
    // Remove stray elements
    cleaned.replace(QRegularExpression("\\$1"), "");
    cleaned.replace(QRegularExpression("<hr\\s*/?>"), "");
    cleaned.replace(QRegularExpression("<br\\s*/?>"), "");

    // Embed CSS and UTF-8 meta
    QString styled = cleaned;
    styled.replace("</head>", "<style>" + css + "</style><meta charset=\"utf-8\" /></head>");
    return styled;
}
