import QtQuick 2.9
import org.asteroid.controls 1.0

Item{
	id: textBox
	property alias inputMethodHints: textInput.inputMethodHints
   property alias validator: textInput.validator
   property alias echoMode: textInput.echoMode
   property string previewText
	property int passwordMaskDelay: 1000
	
	Flickable {
   	id: flickable
		x: 12
      clip: true
      width: parent.width+5
      height: parent.height+5
      flickableDirection: Flickable.HorizontalFlick
      interactive: contentWidth - 4 > width
		contentWidth: textInput.width+2
      contentHeight: textInput.height
	
		Rectangle {
			id: txtField
			y:10
			border.color: "green"
			border.width: 2
			color: "#FFFFFF"
			width:  flickable.width -5
			height: flickable.height -5
	
	 		TextInput {
   			id: textInput
				font.pixelSize : 30
				color: (!textInput.activeFocus) ? "#a0a1a2" : "#2B2C2E"
   			width: flickable.width -5
   			height: flickable.height -5
   			passwordCharacter: "\u2022"
   			text: (!textInput.activeFocus) ? previewText : "" 
   			verticalAlignment: TextInput.AlignVCenter
   			horizontalAlignment: TextInput.AlignLeft
   			wrapMode :TextInput.WordWrap 
   			
   			 Binding {
                target: textInput
                property: "passwordMaskDelay"
                value: 1000
                when: textInput.hasOwnProperty("passwordMaskDelay")
            }
   		}
		}		
	}
}
