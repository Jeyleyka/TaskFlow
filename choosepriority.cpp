#include "choosepriority.h"

ChoosePriority::ChoosePriority(QWidget* parent) :
    QDialog(parent) {
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    this->setFixedSize(400, 556);
    this->setAttribute(Qt::WA_TransparentForMouseEvents);

    QWidget* container = new QWidget(this);
    container->setObjectName("container");
    container->setStyleSheet(R"(
        #container {
            background-color: #2e2e2e;
        }
    )");

    this->containerLayout = new QVBoxLayout(container);

    this->titleWnd = new QLabel(tr("Task Priority"), this);
    this->titleWnd->setStyleSheet("font-size: 15px; color: #fff;");
    this->titleWnd->setAlignment(Qt::AlignHCenter);

    containerLayout->addWidget(this->titleWnd);

    this->line = new QFrame(this);
    this->line->setFrameShape(QFrame::HLine);
    this->line->setFrameShadow(QFrame::Sunken);
    this->line->setStyleSheet("height: 1px; width: 323px; background-color: #5A5A5A;");

    containerLayout->addWidget(this->line);

    for (int i = 1; i < 11; ++i)
        this->widgets.push_back(new PriorityItemWidget(i, this));

    this->widgetsLayout = new FlowLayout;
    this->widgetsLayout->setContentsMargins(0,0,0,0);
    this->widgetsLayout->setSpacing(0);

    for (PriorityItemWidget* widget : this->widgets)
    {
        this->widgetsLayout->addWidget(widget);

        connect(widget, &PriorityItemWidget::clickedWithId, this, [this](int priority) {
            this->prior = priority;
            qDebug() << "нажат с приоритетом: " << priority;
        });
    }

    this->containerLayout->addLayout(this->widgetsLayout);

    this->cancel = new QPushButton(tr("Cancel"), this);
    this->cancel->setStyleSheet("width: 153px; height: 48px; color: #7C7DD1; background-color: transparent; font-size: 13px; border: none;");

    connect(this->cancel, &QPushButton::clicked, this, &ChoosePriority::close);

    this->btnsLayout = new QHBoxLayout;

    this->btnsLayout->addWidget(this->cancel);

    this->save = new QPushButton(tr("Save"), this);
    this->save->setStyleSheet("width: 153px; height: 48px; color: #fff; background-color: #8687E7; font-size: 13px;");

    connect(this->save, &QPushButton::clicked, this, [this] {
        emit prioritySelected(this->prior);
        this->close();
    });

    this->btnsLayout->addWidget(this->save);

    this->containerLayout->addStretch();
    this->containerLayout->addLayout(this->btnsLayout);

    this->mainLayout = new QVBoxLayout(this);
    this->mainLayout->addWidget(container);
    this->mainLayout->setContentsMargins(0,0,0,0);
}

const int& ChoosePriority::getPrority() {
    return this->prior;
}
