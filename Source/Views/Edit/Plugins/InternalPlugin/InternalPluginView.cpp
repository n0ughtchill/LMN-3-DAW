#include "InternalPluginView.h"

InternalPluginView::InternalPluginView(tracktion_engine::Plugin* p, app_services::MidiCommandManager& mcm)
    : viewModel(std::make_unique<app_view_models::InternalPluginViewModel>(p)),
      midiCommandManager(mcm)
{

    init();

}

InternalPluginView::InternalPluginView(tracktion_engine::ReverbPlugin* p, app_services::MidiCommandManager& mcm)
    : viewModel(std::unique_ptr<app_view_models::InternalPluginViewModel>(std::make_unique<app_view_models::ReverbPluginViewModel>(p))),
      midiCommandManager(mcm)
{

    init();

}

InternalPluginView::InternalPluginView(tracktion_engine::DelayPlugin* p, app_services::MidiCommandManager& mcm)
    : viewModel(std::unique_ptr<app_view_models::InternalPluginViewModel>(std::make_unique<app_view_models::DelayPluginViewModel>(p))),
      midiCommandManager(mcm)
{

    init();

}

InternalPluginView::InternalPluginView(tracktion_engine::PhaserPlugin* p, app_services::MidiCommandManager& mcm)
        : viewModel(std::unique_ptr<app_view_models::InternalPluginViewModel>(std::make_unique<app_view_models::PhaserPluginViewModel>(p))),
          midiCommandManager(mcm)
{

    init();

}

void InternalPluginView::init()
{

    titleLabel.setFont(juce::Font(juce::Font::getDefaultMonospacedFontName(), getHeight() * .1, juce::Font::plain));
    titleLabel.setText(viewModel->getPluginName(), juce::dontSendNotification );
    titleLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(titleLabel);

    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;

    int numCols = 4;
    int numRows = 2;

    for (int i = 0; i < numRows; i++)
    {

        grid1.templateRows.add(Track(Fr(1)));
        grid2.templateRows.add(Track(Fr(1)));

    }


    for (int j = 0; j < numCols; j++)
    {

        grid1.templateColumns.add(Track(Fr(1)));
        grid2.templateColumns.add(Track(Fr(1)));

    }


    for (int i = 0; i < viewModel->getNumberOfParameters(); i++)
    {

        knobs.add(new LabeledKnob());
        knobs[i]->getLabel().setText(viewModel->getParameterName(i), juce::dontSendNotification);
        knobs[i]->getSlider().setRange(viewModel->getParameterRange(i), viewModel->getParameterInterval(i));

        if (i == 0 || i == 8)
        {

            knobs[i]->getSlider().setColour(juce::Slider::rotarySliderFillColourId, appLookAndFeel.colour1);
            knobs[i]->getSlider().setColour(juce::Slider::thumbColourId, appLookAndFeel.colour1);

        }

        if (i == 1 || i == 9)
        {

            knobs[i]->getSlider().setColour(juce::Slider::rotarySliderFillColourId, appLookAndFeel.colour2);
            knobs[i]->getSlider().setColour(juce::Slider::thumbColourId, appLookAndFeel.colour2);

        }

        if (i == 2 || i == 10)
        {

            knobs[i]->getSlider().setColour(juce::Slider::rotarySliderFillColourId, appLookAndFeel.colour3);
            knobs[i]->getSlider().setColour(juce::Slider::thumbColourId, appLookAndFeel.colour3);

        }

        if (i == 3 || i == 11)
        {

            knobs[i]->getSlider().setColour(juce::Slider::rotarySliderFillColourId, appLookAndFeel.colour4);
            knobs[i]->getSlider().setColour(juce::Slider::thumbColourId, appLookAndFeel.colour4);

        }

        if (i < 8)
        {

            grid1.items.add(juce::GridItem(knobs[i]));
            addAndMakeVisible(knobs[i]);

        }
        else
        {
            grid2.items.add(juce::GridItem(knobs[i]));
            addChildComponent(knobs[i]);

        }

    }

    midiCommandManager.addListener(this);
    viewModel->addListener(this);

}


InternalPluginView::~InternalPluginView()
{

    midiCommandManager.removeListener(this);
    viewModel->removeListener(this);

}

void InternalPluginView::paint(juce::Graphics& g)
{

}

void InternalPluginView::resized()
{

    titleLabel.setFont(juce::Font(juce::Font::getDefaultMonospacedFontName(), getHeight() * .1, juce::Font::plain));
    titleLabel.setBounds(0, getHeight() * .1, getWidth(), getHeight() * .1);

    gridSetup();

}


void InternalPluginView::gridSetup()
{


    int widthPadding = getWidth() * .05;
    int heightPadding = getHeight() * .05;

    grid1.setGap(juce::Grid::Px(heightPadding));
    grid2.setGap(juce::Grid::Px(heightPadding));

    int startX = widthPadding;
    int startY = titleLabel.getY() + titleLabel.getHeight() + (heightPadding);
    int width = getWidth() - (2 * widthPadding);
    int height = (getHeight() - startY) - (heightPadding);

    juce::Rectangle<int> bounds(startX, startY, width, height);
    grid1.performLayout(bounds);
    grid2.performLayout(bounds);


}

void InternalPluginView::shiftButtonPressed()
{

    if (knobs.size() > 8)
    {

        for (int i = 0; i < knobs.size(); i++)
        {

            if (i < 8)
                knobs[i]->setVisible(false);
            else
                knobs[i]->setVisible(true);

        }

    }

}

void InternalPluginView::shiftButtonReleased()
{

    if (knobs.size() > 8)

    {
        for (int i = 0; i < knobs.size(); i++) {

            if (i < 8)
                knobs[i]->setVisible(true);
            else
                knobs[i]->setVisible(false);

        }

    }

}

void InternalPluginView::encoder1Increased()
{

    if (!midiCommandManager.isShiftDown)
        viewModel->setParameterValue(0, viewModel->getParameterValue(0) + viewModel->getParameterInterval(0));
    else
        viewModel->setParameterValue(8, viewModel->getParameterValue(8) + viewModel->getParameterInterval(8));

}

void InternalPluginView::encoder1Decreased()
{

    if (!midiCommandManager.isShiftDown)
        viewModel->setParameterValue(0, viewModel->getParameterValue(0) - viewModel->getParameterInterval(0));
    else
        viewModel->setParameterValue(8, viewModel->getParameterValue(8) - viewModel->getParameterInterval(8));

}

void InternalPluginView::encoder2Increased()
{

    if (!midiCommandManager.isShiftDown)
        viewModel->setParameterValue(1, viewModel->getParameterValue(1) + viewModel->getParameterInterval(1));
    else
        viewModel->setParameterValue(9, viewModel->getParameterValue(9) + viewModel->getParameterInterval(9));

}

void InternalPluginView::encoder2Decreased()
{

    if (!midiCommandManager.isShiftDown)
        viewModel->setParameterValue(1, viewModel->getParameterValue(1) - viewModel->getParameterInterval(1));
    else
        viewModel->setParameterValue(9, viewModel->getParameterValue(9) - viewModel->getParameterInterval(9));

}

void InternalPluginView::encoder3Increased()
{

    if (!midiCommandManager.isShiftDown)
        viewModel->setParameterValue(2, viewModel->getParameterValue(2) + viewModel->getParameterInterval(2));
    else
        viewModel->setParameterValue(10, viewModel->getParameterValue(10) + viewModel->getParameterInterval(10));

}

void InternalPluginView::encoder3Decreased()
{

    if (!midiCommandManager.isShiftDown)
        viewModel->setParameterValue(2, viewModel->getParameterValue(2) - viewModel->getParameterInterval(2));
    else
        viewModel->setParameterValue(10, viewModel->getParameterValue(10) - viewModel->getParameterInterval(10));

}

void InternalPluginView::encoder4Increased()
{

    if (!midiCommandManager.isShiftDown)
        viewModel->setParameterValue(3, viewModel->getParameterValue(3) + viewModel->getParameterInterval(3));
    else
        viewModel->setParameterValue(11, viewModel->getParameterValue(11) + viewModel->getParameterInterval(11));

}

void InternalPluginView::encoder4Decreased()
{

    if (!midiCommandManager.isShiftDown)
        viewModel->setParameterValue(3, viewModel->getParameterValue(3) - viewModel->getParameterInterval(3));
    else
        viewModel->setParameterValue(11, viewModel->getParameterValue(11) - viewModel->getParameterInterval(11));

}

void InternalPluginView::parametersChanged()
{

    for (int i = 0; i < viewModel->getNumberOfParameters(); i++)
    {

        knobs[i]->getSlider().setValue(viewModel->getParameterValue(i), juce::dontSendNotification);

    }

}