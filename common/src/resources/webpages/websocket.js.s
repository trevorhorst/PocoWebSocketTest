    .section .rodata
    .global WEBSOCKET_JS
    .type   WEBSOCKET_JS, %object
    .align  4
WEBSOCKET_JS:
    .incbin "common/resources/webpages/websocket.js"
WEBSOCKET_JS_END:
    .global WEBSOCKET_JS_SIZE
    .type   WEBSOCKET_JS_SIZE, %object
    .align  4
WEBSOCKET_JS_SIZE:
    .int    WEBSOCKET_JS_END - WEBSOCKET_JS
