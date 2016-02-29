import QtQuick 2.5
import QtQuick.Window 2.2

Window {
    visible: true

    DrawArea {
        id: myArea
        onPtrClicked: {
            publisher.clicked(Qt.point(mouseX, mouseY)); }
        onPosChanged:
            publisher.relayChanges(Qt.point(mouseX, mouseY));
        onPtrReleased:
            publisher.released(Qt.point(mouseX, mouseY));
    }

    Connections {
        target: publisher
        onBlurChanged :  myArea.blurRadius = value;
    }
    Connections {
        target: publisher
        onClear :  myArea.clear();
    }

}
