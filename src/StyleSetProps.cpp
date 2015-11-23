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

#include "StyleSetProps.hpp"

#include "StyleSet.hpp"

namespace aqt
{
namespace stylesheets
{

StyleSetProps::StyleSetProps(StyleSet* pStyleSet)
  : mpStyleSet(pStyleSet)
{
}

void StyleSetProps::setStyleSet(StyleSet* pStyleSet)
{
  mpStyleSet = pStyleSet;
}

bool StyleSetProps::isValid() const
{
  return mpStyleSet && mpStyleSet->isValid();
}

bool StyleSetProps::isSet(const QString& key) const
{
  return mpStyleSet && mpStyleSet->isSet(key);
}

QVariant StyleSetProps::get(const QString& key) const
{
  return mpStyleSet ? mpStyleSet->get(key) : QVariant{};
}

QVariant StyleSetProps::values(const QString& key) const
{
  return mpStyleSet ? mpStyleSet->values(key) : QVariant{};
}

QColor StyleSetProps::color(const QString& key) const
{
  return mpStyleSet ? mpStyleSet->color(key) : QColor{};
}

QFont StyleSetProps::font(const QString& key) const
{
  return mpStyleSet ? mpStyleSet->font(key) : QFont{};
}

double StyleSetProps::number(const QString& key) const
{
  return mpStyleSet ? mpStyleSet->number(key) : 0.0;
}

bool StyleSetProps::boolean(const QString& key) const
{
  return mpStyleSet && mpStyleSet->boolean(key);
}

QString StyleSetProps::string(const QString& key) const
{
  return mpStyleSet ? mpStyleSet->string(key) : QString{};
}

QUrl StyleSetProps::url(const QString& key) const
{
  return mpStyleSet ? mpStyleSet->url(key) : QUrl{};
}

} // namespace stylesheets
} // namespace aqt
