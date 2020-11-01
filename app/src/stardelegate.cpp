#include "stardelegate.h"
#include "starrating.h"
#include "stareditor.h"
#include <loggingcategories.h>

#include <QtWidgets>

void StarDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{
//  qDebug(logDebug()) << "StarDelegate::paint";
//  qDebug(logDebug()) << "index collumn = " << index.column();
//  qDebug(logDebug()) << "index row = " << index.row();
//  qDebug(logDebug()) << "index data = " << index.data();

//  if (index.data().canConvert<StarRating>()) {
//    qDebug(logDebug()) << "StarDelegate::paint 2";

//    StarRating starRating = qvariant_cast<StarRating>(index.data());
    StarRating starRating;

    if (option.state & QStyle::State_Selected)
      painter->fillRect(option.rect, option.palette.highlight());

    starRating.paint(painter, option.rect, option.palette,
                     StarRating::EditMode::ReadOnly);
//  } else {
//    qDebug(logDebug()) << "StarDelegate::paint  else =";
//    QStyledItemDelegate::paint(painter, option, index);
//  }
}


QWidget *StarDelegate::createEditor(QWidget *parent,
                                    const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const
{
  qDebug(logDebug()) << "StarDelegate::createEditor";

  if (index.data().canConvert<StarRating>()) {
    StarEditor *editor = new StarEditor(parent);
    connect(editor, &StarEditor::editingFinished,
            this, &StarDelegate::commitAndCloseEditor);
    return editor;
  }
  return QStyledItemDelegate::createEditor(parent, option, index);
}

void StarDelegate::commitAndCloseEditor()
{
  StarEditor *editor = qobject_cast<StarEditor *>(sender());
  emit commitData(editor);
  emit closeEditor(editor);
}

void StarDelegate::setEditorData(QWidget *editor,
                                 const QModelIndex &index) const
{
  qDebug(logDebug()) << "StarDelegate::setEditorData";

  if (index.data().canConvert<StarRating>()) {

    qDebug(logDebug()) << "StarDelegate::setEditorData if true";

    QVariant tmp =  index.data();
    qDebug(logDebug()) << " index.data() =" << index.data();

    StarRating starRating = qvariant_cast<StarRating>(index.data());
    StarEditor *starEditor = qobject_cast<StarEditor *>(editor);
    starEditor->setStarRating(starRating);
  } else {
    QStyledItemDelegate::setEditorData(editor, index);
  }
}

void StarDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                const QModelIndex &index) const
{
    qDebug(logDebug()) << "StarDelegate::setModelData";

  if (index.data().canConvert<StarRating>()) {
    StarEditor *starEditor = qobject_cast<StarEditor *>(editor);
    model->setData(index, QVariant::fromValue(starEditor->starRating()));
  } else {
    QStyledItemDelegate::setModelData(editor, model, index);
  }
}
QSize StarDelegate::sizeHint(const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
  qDebug(logDebug()) << "StarDelegate::sizeHint";
  if (index.data().canConvert<StarRating>()) {
    StarRating starRating = qvariant_cast<StarRating>(index.data());
    return starRating.sizeHint();
  }
  return QStyledItemDelegate::sizeHint(option, index);
}