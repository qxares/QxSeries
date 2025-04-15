#include "cleanhtmlbrick.h"
#include <QDebug>
#include <QRegularExpression>

CleanHtmlBrick::CleanHtmlBrick(QObject *parent) : QObject(parent) {
    css = "body { font-family: Liberation Sans, sans-serif; margin: 0; padding: 0; font-size: 14pt; line-height: 1.0; }\n"
          "p { margin: 0; padding: 0; }\n"
          "p:empty { display: none; }\n"
          "p.empty { display: none; }\n"
          "strong { font-weight: bold; }\n"
          "em { font-style: italic; }\n"
          "table { border-collapse: collapse; width: auto; margin: 0; padding: 0; font-size: 14pt; border: 1px solid black; }\n"
          "td, th { border: 1px solid black; padding: 5px; text-align: left; font-size: 14pt; font-family: Liberation Sans; }\n";
    qDebug() << "CleanHtmlBrick initialized";
}

QString CleanHtmlBrick::cleanHtml(const QString &html) {
    QString cleaned = html;
    // Strip HTML structure
    cleaned.replace(QRegularExpression("<!DOCTYPE[^>]*>"), "");
    cleaned.replace(QRegularExpression("<html[^>]*>"), "");
    cleaned.replace(QRegularExpression("</html>"), "");
    cleaned.replace(QRegularExpression("<head[^>]*>.*?</head>"), "");
    cleaned.replace(QRegularExpression("<body[^>]*>"), "");
    cleaned.replace(QRegularExpression("</body>"), "");
    // Preserve bold/italic explicitly
    cleaned.replace(QRegularExpression("<b\\b[^>]*>"), "<strong>");
    cleaned.replace(QRegularExpression("</b>"), "</strong>");
    cleaned.replace(QRegularExpression("<i\\b[^>]*>"), "<em>");
    cleaned.replace(QRegularExpression("</i>"), "</em>");
    // Strip Qt noise
    cleaned.replace(QRegularExpression("<h[1-6][^>]*>.*?</h[1-6]>"), "");
    cleaned.replace(QRegularExpression("data-[^=]+=\"[^\"]+\""), "");
    cleaned.replace(QRegularExpression("<title>.*</title>"), "");
    // Preserve spans with bold/italic styles
    cleaned.replace(QRegularExpression("<span\\b[^>]*style=\"([^\"]*font-weight:600[^\"]*)\"[^>]*>(.*?)</span>"), "<strong>\\2</strong>");
    cleaned.replace(QRegularExpression("<span\\b[^>]*style=\"([^\"]*font-style:italic[^\"]*)\"[^>]*>(.*?)</span>"), "<em>\\2</em>");
    // Table cleanup: preserve attributes
    cleaned.replace(QRegularExpression("<table\\s*[^>]*>"), "<table border=\"1\" cellspacing=\"0\" cellpadding=\"5\" style=\"width: auto; border-collapse: collapse; margin: 0; padding: 0; font-size: 14pt;\">");
    cleaned.replace(QRegularExpression("<td\\s*[^>]*>"), "<td style=\"border: 1px solid black; padding: 5px; text-align: left; font-size: 14pt; font-family: Liberation Sans;\">");
    // Paragraph cleanup
    cleaned.replace(QRegularExpression("<p\\s*[^>]*></p>"), "");
    cleaned.replace(QRegularExpression("<p\\s*[^>]*>\\s*</p>"), "");
    cleaned.replace(QRegularExpression("<p\\s*[^>]*>\\s* \\s*</p>"), "");
    cleaned.replace(QRegularExpression("<p\\s*[^>]*>(\\s| )*</p>"), "");
    // Remove stray elements
    cleaned.replace(QRegularExpression("<br\\s*/?>"), "");
    cleaned.replace(QRegularExpression("<hr\\s*/?>"), "");
    cleaned.replace(QRegularExpression("\\$1"), "");
    // Ensure font size
    cleaned.replace(QRegularExpression("font-size:\\d+pt"), "font-size:14pt");
    // Strip only empty spans
    cleaned.replace(QRegularExpression("<span\\s*[^>]*></span>"), "");

    // Embed CSS only for non-RTF formats
    QString styled = cleaned;
    if (!html.contains("rtf", Qt::CaseInsensitive)) {
        styled.replace(QRegularExpression("<\\?xml[^>]*\\?>"), "");
        styled = "<html><head><style>" + css + "</style><meta charset=\"utf-8\" /></head><body>" + styled + "</body></html>";
    }
    qDebug() << "CleanHtmlBrick: Cleaned HTML preview:" << styled.left(200);
    return styled;
}
