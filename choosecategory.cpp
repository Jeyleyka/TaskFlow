#include "choosecategory.h"

ChooseCategory::ChooseCategory(QWidget* parent)
    : QDialog(parent), selectedCategory(nullptr) {
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    this->setFixedSize(327,556);

    QWidget* container = new QWidget(this);
    container->setObjectName("container");
    container->setStyleSheet(R"(
        #container {
            background-color: #2e2e2e;
            border-radius: 15px;
        }
    )");

    QVBoxLayout* containerLayout = new QVBoxLayout(container);

    this->wndTitle = new QLabel(tr("Choose Category"), this);
    this->wndTitle->setStyleSheet("font-size: 15px; color: #fff;");
    this->wndTitle->setAlignment(Qt::AlignHCenter);

    containerLayout->addWidget(this->wndTitle);

    this->line = new QFrame(this);
    this->line->setFrameShape(QFrame::HLine);
    this->line->setFrameShadow(QFrame::Sunken);
    this->line->setStyleSheet("height: 1px; width: 323px; background-color: #5A5A5A;");

    containerLayout->addWidget(this->line);

    this->editor = new CategoryEditor(this);
    // this->editor->show();
    connect(this->editor, &CategoryEditor::categoryCreated,
            this, &ChooseCategory::addCategory);

    this->listWidget = new QListWidget(this);
    this->listWidget->setViewMode(QListView::IconMode);
    this->listWidget->setResizeMode(QListView::Adjust);
    this->listWidget->setSpacing(12);
    this->listWidget->setMovement(QListView::Static);
    this->listWidget->setFlow(QListView::LeftToRight);

    containerLayout->addWidget(this->listWidget);

    this->addCategoryBtn = new QPushButton(tr("Add Category"), this);
    this->addCategoryBtn->setStyleSheet("width: 315px; height: 48px; color: #fff; background-color: #8687E7; font-size: 13px;");
    this->addCategoryBtn->setCheckable(true);

    connect(this->listWidget, &QListWidget::itemClicked, this, [this](QListWidgetItem* item) {
        // Получить виджет, связанный с элементом
        QWidget* widget = this->listWidget->itemWidget(item);
        if (!widget) return;

        // Преобразуем в наш виджет категории
        auto* categoryWidget = qobject_cast<CategoryItemWidget*>(widget);
        if (!categoryWidget) return;

        // Сравниваем название
        if (categoryWidget->getName() == "Create new") {
            this->editor->show();
            return;
        }

        this->selectedCategory = categoryWidget;
    });

    connect(this->addCategoryBtn, &QPushButton::clicked, this, [this] {
        if (this->selectedCategory)
        {
            emit categorySelected(this->selectedCategory->getName(), this->selectedCategory->getColor(), this->selectedCategory->getIcon());
            this->close();
        }
    });

    containerLayout->addWidget(this->addCategoryBtn);

    this->mainLayout = new QVBoxLayout(this);
    this->mainLayout->addWidget(container);
    this->mainLayout->setContentsMargins(0,0,0,0);

    // 🔽 Перенеси это в конец
    QSqlQuery query;
    query.prepare("SELECT name, color, icon FROM categories WHERE user_id = :user_id OR user_id = 0");

    query.bindValue(":user_id", UserSession::getUserId());

    if (!query.exec()) {
        qDebug() << "Ошибка загрузки категорий:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        QString name = query.value(0).toString();
        QColor color(query.value(1).toString());
        QByteArray iconData = query.value(2).toByteArray();

        QPixmap pixmap;
        pixmap.loadFromData(iconData);
        QIcon icon(pixmap);

        this->addCategoryToUI(name, color, icon);  // только UI
    }
}

void ChooseCategory::addCategory(const QString &name, const QColor &color, const QIcon &icon) {
    // Проверка: есть ли уже категория с таким именем
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM categories WHERE name = :name AND user_id = :user_id");
    checkQuery.bindValue(":name", name);
    checkQuery.bindValue(":user_id", UserSession::getUserId());

    if (!checkQuery.exec()) {
        qDebug() << "Ошибка проверки на дубликат:" << checkQuery.lastError();
        return;
    }

    if (checkQuery.next() && checkQuery.value(0).toInt() > 0) {
        WarningWnd* warning = new WarningWnd(tr("Category with this name already exists"), this);
        warning->showWithAnimation();
        return; // Не добавляем дубликат
    }

    // Добавляем в UI
    addCategoryToUI(name, color, icon);

    // Сохраняем в базу
    QSqlQuery query;
    query.prepare("INSERT INTO categories (user_id, name, color, icon) VALUES (:user_id, :name, :color, :icon)");

    QPixmap pixmap = icon.pixmap(64, 64);
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");

    query.bindValue(":user_id", UserSession::getUserId());
    query.bindValue(":name", name);
    query.bindValue(":icon", bytes);
    query.bindValue(":color", color.name());

    if (name == "Create new")
        query.bindValue(":user_id", 0);

    if (!query.exec()) {
        qDebug() << "Ошибка добавления категории:" << query.lastError();
    }
}

void ChooseCategory::addCategoryToUI(const QString &name, const QColor &color, const QIcon &icon) {
    QListWidgetItem* item = new QListWidgetItem(this->listWidget);
    item->setSizeHint(QSize(80, 100));

    auto* widget = new CategoryItemWidget(name, color, icon);
    this->listWidget->addItem(item);
    this->listWidget->setItemWidget(item, widget);
}

void ChooseCategory::onAddCategory() {
    emit this->AddCategorySignal();
}
