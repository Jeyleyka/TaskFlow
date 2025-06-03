#include "edittask.h"

EditTask::EditTask(QString titleStr, QString descStr, QString category_name, QColor category_color, QIcon category_icon, int priori, QWidget* parent)
    : QDialog(parent) {

    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    this->setFixedSize(327, 385);
    this->installEventFilter(this);

    QWidget* container = new QWidget(this);
    container->setObjectName("container");
    container->setStyleSheet(R"(
        #container {
            background-color: #2e2e2e;
            border-radius: 15px;
        }
    )");

    QVBoxLayout* containerLayout = new QVBoxLayout(container);

    this->wndTitle = new QLabel(tr("Edit Task"), this);
    this->wndTitle->setStyleSheet("line-height: 20px; font-size: 14px; color: #fff; margin-bottom: 5px;");
    this->wndTitle->setAlignment(Qt::AlignHCenter);

    containerLayout->addWidget(this->wndTitle);

    this->line = new QFrame(this);
    this->line->setFrameShape(QFrame::HLine);
    this->line->setFrameShadow(QFrame::Sunken);
    this->line->setStyleSheet("height: 1px; width: 312px; background-color: #5A5A5A;");

    containerLayout->addWidget(this->line);

    this->title = new QLabel(titleStr, this);
    this->title->setStyleSheet("font-size: 17px; color: white; max-height: 30px; margin-top: 10px;");
    this->title->installEventFilter(this);

    containerLayout->addWidget(this->title);

    this->editTitle = new QLineEdit(titleStr, this);
    this->editTitle->setStyleSheet("font-size: 19px; color: white; height: 40px; margin-top: 10px; border: 1px solid #727272; border-radius: 5px; padding-left: 10px;");
    this->editTitle->hide();

    connect(this->editTitle, &QLineEdit::returnPressed, this, [this] {
        this->title->setText(this->editTitle->text());
        this->editTitle->hide();
        this->title->show();

        if (!this->editDesc->isHidden())
            this->editTitle->hide();
    });

    containerLayout->addWidget(this->editTitle);

    this->categoryWidget = new CategoryItemWidget(category_name, category_color, category_icon);

    connect(this->categoryWidget, &CategoryItemWidget::itemClicked, this, [this, containerLayout] {
        this->categoryWnd = new ChooseCategory(this);
        this->categoryWnd->show();

        connect(this->categoryWnd, &ChooseCategory::categorySelected, this, [this, containerLayout](const QString& name, const QColor& color, const QIcon& icon) {
            if (name == "Create new")
                return;

            if (this->categoryWidget)
            {
                containerLayout->removeWidget(this->categoryWidget);
                containerLayout->removeWidget(this->priorityWidget);
                containerLayout->removeWidget(this->description);
                containerLayout->removeWidget(this->editDesc);
                containerLayout->removeItem(this->btnsLayout);
                this->categoryWidget->deleteLater();
                this->categoryWidget = nullptr;
            }

            categoryName = name;
            categoryColor = color;
            categoryIcon = icon;
            qDebug() << "path: " << this->categoryIcon;

            this->categoryWidget = new CategoryItemWidget(name, color, icon);
            containerLayout->addWidget(this->categoryWidget, 0, Qt::AlignLeft);
            containerLayout->addWidget(this->priorityWidget, 0, Qt::AlignLeft);
            containerLayout->addWidget(this->description);
            containerLayout->addWidget(this->editDesc);
            containerLayout->addLayout(this->btnsLayout);
        });
    });

    containerLayout->addWidget(this->categoryWidget, 0, Qt::AlignLeft);

    this->priorityWidget = new PriorityItemWidget(priori, this);
    containerLayout->addWidget(this->priorityWidget, 0, Qt::AlignLeft);

    connect(this->priorityWidget, &PriorityItemWidget::openPriorityWnd, this, [this, containerLayout] {
        this->priorityWnd = new ChoosePriority;
        this->priorityWnd->show();

        connect(this->priorityWnd, &ChoosePriority::prioritySelected, this, [this, containerLayout](int priori) {
            if (this->priorityWidget)
            {
                containerLayout->removeWidget(this->priorityWidget);
                containerLayout->removeWidget(this->description);
                containerLayout->removeWidget(this->editDesc);
                containerLayout->removeItem(this->btnsLayout);
                this->priorityWidget->deleteLater();
                this->priorityWidget = nullptr;
            }

            this->priorityWidget = new PriorityItemWidget(priori, this);

            containerLayout->addWidget(this->priorityWidget, 0, Qt::AlignLeft);
            containerLayout->addWidget(this->description);
            containerLayout->addWidget(this->editDesc);
            containerLayout->addLayout(this->btnsLayout);
        });
    });

    this->description = new QLabel(descStr, this);
    this->description->setStyleSheet("font-size: 14px; color: #cccccc; max-height: 30px; margin-top: 15px;");
    this->description->installEventFilter(this);

    containerLayout->addWidget(this->description);

    this->editDesc = new QLineEdit(descStr, this);
    this->editDesc->setStyleSheet("font-size: 16px; color: #cccccc; height: 40px; margin-top: 15px; border: 1px solid #727272; border-radius: 5px; padding-left: 10px;");
    this->editDesc->hide();

    connect(this->editDesc, &QLineEdit::returnPressed, this, [this] {
        this->description->setText(this->editDesc->text());
        this->editDesc->hide();
        this->description->show();

        if (!this->editTitle->isHidden())
            this->editTitle->hide();
    });

    containerLayout->addWidget(this->editDesc);
    containerLayout->addStretch();

    this->btnsLayout = new QHBoxLayout(container);

    this->cancel = new QPushButton(tr("Cancel"), this);
    this->cancel->setStyleSheet("width: 153px; height: 48px; color: #7C7DD1; background-color: transparent; font-size: 13px; border: none;");

    connect(this->cancel, &QPushButton::clicked, this, &EditTask::close);

    this->btnsLayout->addWidget(this->cancel);

    this->edit = new QPushButton(tr("Edit"), this);
    this->edit->setStyleSheet("width: 153px; height: 48px; color: #fff; background-color: #8687E7; font-size: 13px;");

    connect(this->edit, &QPushButton::clicked, this, &EditTask::onUpdateData);

    this->btnsLayout->addWidget(this->edit);

    containerLayout->addLayout(this->btnsLayout, 5);

    this->mainLayout = new QVBoxLayout(this);
    this->mainLayout->addWidget(container);
    this->mainLayout->setContentsMargins(0, 0, 0, 0);
}

QString EditTask::text() const {
    return this->title->text();
}

QString EditTask::getDescription() const {
    return this->description->text();
}

QString EditTask::getPriority() const {
    return QString::number(this->priorityWidget->getPriority());
}

QString EditTask::getCategoryName() const {
    return this->categoryName;
}

QColor EditTask::getCategoryColor() const {
    return this->categoryColor;
}

QIcon EditTask::getCategoryIcon() const {
    return this->categoryWidget->getIcon();
}

void EditTask::setText(const QString &text) {
    this->title->setText(text);
    this->editTitle->setText(text);
}

void EditTask::enterEditMode(QLabel* label, QLineEdit* editor) {
    label->hide();
    editor->setText(label->text());
    editor->show();
    editor->setFocus();
    editor->selectAll();
}

void EditTask::leaveEditMode(QLabel* label, QLineEdit* editor) {
    label->setText(editor->text());
    emit textEdited(editTitle->text());

    editor->hide();
    label->show();
}

void EditTask::onUpdateData() {
    emit updateEdit();
    this->close();
}

bool EditTask::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::MouseButtonPress) {
        if (obj == this->title) {
            // Скрыть description, если он редактируется
            if (this->editDesc->isVisible()) {
                this->editDesc->hide();
                this->description->show();
            }

            // Переключаем title в режим редактирования
            this->title->hide();
            this->editTitle->setText(this->title->text());
            this->editTitle->show();
            this->editTitle->setFocus();
            return true;
        }

        if (obj == this->description) {
            // Скрыть title, если он редактируется
            if (this->editTitle->isVisible()) {
                this->editTitle->hide();
                this->title->show();
            }

            // Переключаем description в режим редактирования
            this->description->hide();
            this->editDesc->setText(this->description->text());
            this->editDesc->show();
            this->editDesc->setFocus();
            return true;
        }

        if (event->type() == QEvent::MouseButtonPress) {
            if (obj != this->title && obj != this->editTitle && this->editTitle->isVisible()) {
                this->editTitle->hide();
                this->title->show();
            }

            if (obj != this->description && obj != this->editDesc && this->editDesc->isVisible()) {
                this->editDesc->hide();
                this->description->show();
            }
        }
    }

    return QDialog::eventFilter(obj, event);
}

void EditTask::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        event->accept(); // блокируем стандартное поведение
        return;
    }
    QDialog::keyPressEvent(event);
}


