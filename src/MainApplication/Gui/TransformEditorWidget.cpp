#include <MainApplication/Gui/TransformEditorWidget.hpp>
#include <QVBoxLayout>
#include <MainApplication/Gui/VectorEditor.hpp>
#include <MainApplication/Gui/RotationEditor.hpp>

namespace Ra
{
    namespace Gui
    {

        TransformEditorWidget::TransformEditorWidget(QWidget* parent)
                : QWidget(parent), m_currentEdit(nullptr), m_layout(new QVBoxLayout(this)) { }

        void TransformEditorWidget::updateValues()
        {
            CORE_ASSERT(m_transform.primitives.size() == m_widgets.size(), "Widget/props inconsistency");
            for (uint p = 0; p < m_transform.primitives.size(); ++p)
            {
                const Engine::EditablePrimitive& prim = m_transform.primitives[p].primitive;
                switch (prim.getType())
                {
                    case Engine::EditablePrimitive::POSITION:
                    {
                        CORE_ASSERT(m_widgets[p], "No widget for property");
                        static_cast<VectorEditor*>( m_widgets[p] )->setValue(prim.asPosition());
                        break;
                    }
                    case Engine::EditablePrimitive::ROTATION:
                    {
                        CORE_ASSERT(m_widgets[p], "No widget for property");
                        static_cast<RotationEditor*>( m_widgets[p] )->setValue(prim.asRotation());
                        break;
                    }
                    default:;// do nothing.
                }
            }
        }

        TransformEditorWidget::~TransformEditorWidget()
        {
            // This will delete the widgets if there were any.
            setEditable(nullptr);
        }

        void TransformEditorWidget::onChangedPosition(uint id, const Core::Vector3& v)
        {
            CORE_ASSERT(m_currentEdit, "Nothing to edit");
            CORE_ASSERT(sender() == m_widgets[id], "Sender error");
            m_transform.primitives[id].primitive.asPosition() = v;
            m_currentEdit->setProperty(m_transform);
        }

        void TransformEditorWidget::onChangedRotation(uint id, const Core::Quaternion& q)
        {
            CORE_ASSERT(m_currentEdit, "Nothing to edit");
            CORE_ASSERT(sender() == m_widgets[id], "Sender error");
            m_transform.primitives[id].primitive.asRotation() = q;
            m_currentEdit->setProperty(m_transform);
        }

        void TransformEditorWidget::setEditable(Engine::EditableInterface* edit)
        {
            if (m_currentEdit != edit)
            {
                m_currentEdit = edit;
                // Delete widgets from the previous editable.
                for (auto w : m_widgets)
                {
                    delete w;
                }
                m_widgets.clear();

                if (m_currentEdit)
                {
                    Core::AlignedStdVector<Engine::EditableProperty> props;
                    m_currentEdit->getProperties(props);

                    int transformFound = -1;
                    for (uint i = 0; i < props.size(); ++i)
                    {
                        if (props[i].type == Engine::EditableProperty::TRANSFORM)
                        {
                            transformFound = i;
                            m_transform = props[i];
                            break;
                        }
                    }
                    if (transformFound >= 0)
                    {
                        uint id = 0;
                        for (auto p : props[transformFound].primitives)
                        {
                            switch (p.primitive.getType())
                            {
                                case Engine::EditablePrimitive::POSITION:
                                {
                                    VectorEditor* widget = new VectorEditor(id, QString::fromStdString( p.primitive.getName()),
                                                                            p.isEditable);
                                    m_layout->addWidget(widget);
                                    widget->setValue(p.primitive.asPosition());
                                    connect(widget, SIGNAL(valueChanged(uint,
                                                                   const Core::Vector3& )),
                                            this, SLOT(onChangedPosition(uint,
                                                               const Core::Vector3& )));
                                    m_widgets.push_back(widget);
                                    break;
                                }
                                case Engine::EditablePrimitive::ROTATION:
                                {
                                    RotationEditor* widget = new RotationEditor(id,
                                                                                QString::fromStdString(p.primitive.getName()),
                                                                                p.isEditable);
                                    m_layout->addWidget(widget);
                                    widget->setValue(p.primitive.asRotation());
                                    connect(widget, SIGNAL(valueChanged(uint,
                                                                   const Core::Quaternion& )),
                                            this, SLOT(onChangedRotation(uint,
                                                               const Core::Quaternion& )));
                                    m_widgets.push_back(widget);
                                    break;
                                }
                                default:
                                    m_widgets.push_back(nullptr);
                            }
                            ++id;
                        }
                        CORE_ASSERT(m_widgets.size() == m_transform.primitives.size(), "Widget/property inconsistency");
                    }
                }
            }

        }
    }
}