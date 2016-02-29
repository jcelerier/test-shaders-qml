import QtQuick 2.0

import QtGraphicalEffects 1.0

Rectangle {
    width: 900
    height: 900

    property int xpos
    property int ypos

    property int xpos_prev
    property int ypos_prev

    property real blurRadius : 8

    property bool wasCleared : false

    signal ptrClicked(real mouseX, real mouseY)
    signal posChanged(real mouseX, real mouseY)
    signal ptrReleased(real mouseX, real mouseY)

    signal clear
    onClear: {
        wasCleared = true
        myCanvas.requestPaint()
    }

    property double hue : 0.3

    Timer {
        id: myTimer
        repeat: true
        interval: 1
        running: true
        onTriggered: {
            if(myMouseArea.pressed)
            {
                hue = 4 * hue *  ( 1 - hue )
                myShader.qt_Hue = hue
                xpos_prev = xpos
                ypos_prev = ypos
                xpos = myMouseArea.mouseX
                ypos = myMouseArea.mouseY
                myCanvas.requestPaint()
            }
        }
    }

    Canvas {
        id: myCanvas
        anchors.fill: parent

        onPaint: {
            var ctx = getContext('2d')
            if(wasCleared) {
                ctx.clearRect(0, 0, width, height);
                wasCleared = false
            }

            ctx.strokeStyle = 'hsl(' +  ~~(hue * 360)  + ', 50%, 50%)';

            ctx.beginPath();
            if(xpos != xpos_prev || ypos != ypos_prev)
            {
                ctx.moveTo(xpos_prev,ypos_prev);
            }
            else
            {
                ctx.moveTo(xpos_prev - 1,ypos_prev + 1);
            }

            ctx.lineTo(xpos,ypos);
            ctx.lineWidth = 12;
            context.lineJoin = 'round';
            ctx.lineCap = 'round';
            ctx.stroke();
        }

        MouseArea{
            id: myMouseArea
            anchors.fill: parent
            onPressed: {
                myTimer.start()
                xpos = mouseX
                ypos = mouseY
                xpos_prev = xpos
                ypos_prev = ypos
                myCanvas.requestPaint()
                ptrClicked(mouseX, mouseY)
            }

            onMouseXChanged: {
                myCanvas.requestPaint()
                posChanged(mouseX, mouseY)
            }

            onMouseYChanged: {
                myCanvas.requestPaint()
                posChanged(mouseX, mouseY)
            }

            onReleased: {
                myTimer.stop()
                ptrReleased(mouseX, mouseY)
            }
        }
    }

    ShaderEffect {
               id: myShader
               width: 900; height: 900
               property real qt_Opacity: 0.1
               property real qt_Hue: 0.5
               property variant src: ShaderEffectSource {
                   sourceItem: myBlur
               }
               blending: true

               vertexShader: "
                   uniform highp mat4 qt_Matrix;
                   attribute highp vec4 qt_Vertex;
                   attribute highp vec2 qt_MultiTexCoord0;
                   varying highp vec2 coord;
                   void main() {
                       coord = qt_MultiTexCoord0;
                       gl_Position = qt_Matrix * qt_Vertex;
                   }"
               fragmentShader: "
                   varying highp vec2 coord;
                   uniform sampler2D src;
                   uniform lowp float qt_Opacity;
                   uniform lowp float qt_Hue;
                   void main() {
                       lowp vec4 tex = texture2D(src, coord);
                      // tex.r = qt_Hue;

                       gl_FragColor = vec4(tex.r, tex.g, tex.g, 0.8f);;
                   }"
           }


    Blend {
        id: myBlend
        anchors.fill: myShader
        source: myCanvas
        foregroundSource: myShader
        mode: "color"
    }

    GaussianBlur {
        id: myBlur
        source: myBlend
        anchors.fill: myBlend
        radius: blurRadius
        samples: 16
    }

}
