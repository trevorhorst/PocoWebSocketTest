    .section .rodata
    .global INDEX_HTML
    .type   INDEX_HTML, %object
    .align  4
INDEX_HTML:
    .incbin "common/resources/webpages/index.html"
INDEX_HTML_END:
    .global INDEX_HTML_SIZE
    .type   INDEX_HTML_SIZE, %object
    .align  4
INDEX_HTML_SIZE:
    .int    INDEX_HTML_END - INDEX_HTML
