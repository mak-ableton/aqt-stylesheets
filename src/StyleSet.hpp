/*
Copyright (c) 2014-15 Ableton AG, Berlin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#pragma once

#include "Property.hpp"
#include "StyleMatchTree.hpp"
#include "StyleSetProps.hpp"
#include "Warnings.hpp"

SUPPRESS_WARNINGS
#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtCore/QVariant>
#include <QtGui/QColor>
#include <QtGui/QFont>
#include <QtQml/qqml.h>
RESTORE_WARNINGS

class QQuickItem;

namespace aqt
{
namespace stylesheets
{

class StyleEngine;
class StyleSetAttached;

/*! @cond DOXYGEN_IGNORE */

class StyleSet : public QObject
{
  Q_OBJECT

public:
  StyleSet(QObject* pParent = nullptr);

  void initStyleSet(const UiItemPath& path, StyleEngine* pEngine);

  bool isValid() const;
  bool isSet(const QString& key) const;

  QVariant get(const QString& key) const;
  QVariant values(const QString& key) const;

  QColor color(const QString& key) const;
  bool boolean(const QString& key) const;
  double number(const QString& key) const;
  QFont font(const QString& key) const;
  QString string(const QString& key) const;
  QUrl url(const QString& key) const;

  void loadProperties(QObject* pRefObject);

  const PropertyMap& properties(int changeCount);

public Q_SLOTS:
  void onStyleChanged(int changeCount);

private:
  QObject* grandParent();
  bool getImpl(Property& def, const QString& key) const;

  template <typename T>
  T lookupProperty(const QString& key) const;
  template <typename T>
  T lookupProperty(Property& def, const QString& key) const;

private:
  friend class StyleSetAttached;

  QPointer<StyleEngine> mpEngine;
  UiItemPath mPath;
  PropertyMap mProperties;
  int mChangeCount;
};

/*! @endcond */

/*! An attached type used for accessing CSS like style settings
 *
 * StyleSet is the type to access the style properties and information for a
 * QML item.  When a StyleSet is instantiated and attached to
 * a QML item it
 *
 *   - determines the path of the attached-to-item up to its very root (the root
 *     of the QQuickItem object hierarchy).
 *   - connects itself to the singleton styleEngine or its styleChanged signal resp.
 *   - determines its style properties by matching the attached-to item's element
 *     path against the selector rules loaded from the style sheet.
 *
 * A style set can be queried for properties using the props property's
 * various functions, like color(), font(), etc.
 *
 * @par Example:
 * @code
 * Rectangle {
 *   StyleSet.name: "gridline"
 *   color: StyleSet.props.color("background-color")
 * }
 * @endcode
 *
 * @par The Element Path (UiItemPath)
 *
 * The element path is constructed from the typenames and attached "style
 * class" names of the QML and QQuickItem object tree.  In the following
 * example the path for each item is added as a comment:
 *
 * @code
 * Rectangle {               // QQuickRectangle
 *   Text {                  // QQuickRectangle QQuickText
 *   }
 *   Item {                  // QQuickRectangle QQuickItem
 *     ListView {            // QQuickRectangle QQuickItem QQuickListView
 *       delegate: MyView {  // QQuickRectangle QQuickItem QQuickListView MyView
 *       }
 *     }
 *   }
 * }
 * @endcode
 *
 * MyView is a user type defined in the module's qmldir file as MyView.
 * Built-in types like Rectangle or Text begin with @c QQuick.  For
 * debugging purposes the path of an item can be printed to stdout with the
 * StyleSet::path property.
 *
 * @note Matching, determining element path and properties may be delayed in case
 * the global style engine has not been initialized yet (e.g. because it is
 * created lazily from a loader).  In this case using style property lookup
 * functions like get(const QString&) const or
 * color(const QString&, const QColor&) const may not
 * return expected values until the StyleEngine is actually initialized.
 *
 * @par Import in QML:
 * ```import Aqt.StyleSheets 1.0```
 * @since 1.0
 */
class StyleSetAttached : public QObject
{
  Q_OBJECT

  /*! @public Contains the style class name
   *
   * The style class name is part of the element path used for matching selector
   * rules.  This name forms the "dot" part of an elements selector name.
   *
   * @par Example:
   * @code
   * Item {
   *   StyleSet.name: "root"
   *   Component.onCompleted: console.log(StyleSet.path);
   * }
   *
   * => QQuickItem.root
   * @endcode
   *
   * It is possible to define multiple class names for the same item by
   * separating them with whitespace.
   *
   * @par Example:
   * @code
   * Item {
   *   StyleSet.name: "example colors"
   *   Component.onCompleted: console.log(StyleSet.path);
   * }
   *
   * => QQuickItem.{example,colors}
   * @endcode
   *
   * The properties for all matching selectors are merged.
   */
  Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

  /*! @public Contains the path for the element this StyleSet is attached to.
   *
   * The path will look similar to this:
   *
   * ```QQuickRectangle.root/QQuickRowLayout/QQuickItem.box/QQuickText```
   *
   * and reads from root to leaf, i.e. the left most token is the element at
   * the root of the QML object hierarchy.  The path is mostly useful for
   * debugging purposes.
   */
  Q_PROPERTY(QString path READ path NOTIFY pathChanged)

  /*! @public Contains the style properties for the element this StyleSet is
   * attached to
   *
   * @see aqt::stylesheets::StyleSetProps
   *
   * @par Example:
   * @code
   * Text {
   *     color: StyleSet.props.color("color")
   * }
   * @endcode
   *
   */
  Q_PROPERTY(QVariant props READ props NOTIFY propsChanged)

  /*! @cond DOXYGEN_IGNORE */
  Q_PROPERTY(QString styleInfo READ styleInfo NOTIFY propsChanged)

public:
  explicit StyleSetAttached(QObject* pParent = nullptr);

  static StyleSetAttached* qmlAttachedProperties(QObject* pObject);

  QString name() const;
  void setName(const QString& val);

  QString path() const;
  QVariant props();

  StyleSet* style();

  QString styleInfo() const;

  void updateStyle();

/*! @endcond */

Q_SIGNALS:
  /*! Fires when properties change
   *
   * When ever the StyleEngine reloads its style sheet and property values are
   * changed or new properties appear this signal will be fired.
   */
  void propsChanged();

  void nameChanged(const QString& name);

  /*! Fires when the path of the item changes
   *
   * Whenever the location of the object his StyleSet is attached to changes
   * the StyleSet will fire this signal.
   */
  void pathChanged(const QString& path);

  /*! @cond DOXYGEN_IGNORE */

private Q_SLOTS:
  void onStyleEngineChanged(StyleEngine* pEngine);
  void onParentChanged(QQuickItem* pNewParent);

private:
  void setEngine(StyleEngine* pEngine);
  void setupStyle();

private:
  QPointer<StyleEngine> mpEngine;
  StyleSet mStyle;
  StyleSetProps mProps;
  QString mName;
  UiItemPath mPath;

  /*! @endcond */
};

} // namespace stylesheets
} // namespace aqt

QML_DECLARE_TYPEINFO(aqt::stylesheets::StyleSetAttached, QML_HAS_ATTACHED_PROPERTIES)

// include the header implementations
#include "StyleSet.ipp"
