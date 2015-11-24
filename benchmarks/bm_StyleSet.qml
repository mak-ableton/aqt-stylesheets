// Copyright (c) 2015 Ableton AG, Berlin

import QtQuick 2.3
import QtTest 1.0

import Aqt.StyleSheets 1.1

Item {
    id: scene

    TestCase {
        id: tst_siblings
        name: "siblings"

        StyleEngine {
            styleSheetSource: "bm_StyleSet.css"
        }

        QtObject {
            id: globalProperties
            property QtObject props: QtObject {
                property color color: "red"
            }
        }

        Component {
            id: siblings

            Item {
                id: rootItem
                StyleSet.name: "A"

                property string mode

                Item {
                    StyleSet.name: "B"

                    Item {
                        StyleSet.name: "C"

                        Item {
                            StyleSet.name: "D"

                            Repeater {
                                model: 10000

                                property Component simpleBindingRect: Rectangle {
                                    anchors.fill: parent
                                    color: globalProperties.props.color
                                }

                                property Component styleSetBindingRect: Rectangle {
                                    anchors.fill: parent
                                    color: StyleSet.props.color("background")
                                }

                                delegate: rootItem.mode == "styleSet"
                                    ? styleSetBindingRect
                                    : simpleBindingRect
                            }
                        }
                    }
                }
            }
        }

        function benchmark_once_creationOfSiblingsWithSimpleBinding() {
            var obj = siblings.createObject(tst_siblings, { mode: "simple" });
            obj.destroy();
        }

        function benchmark_once_creationOfSiblingsWithStyleSetBinding() {
            var obj = siblings.createObject(tst_siblings, { mode: "styleSet" });
            obj.destroy();
        }

        function cleanup() {
            gc();
        }
    }
}
