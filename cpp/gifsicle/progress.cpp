#include <progress.h>

#include <map>
#include <math.h>

#include <iostream>

ProgressManager::ProgressManager()
: m_states_count(0)
, m_images_count(0)
, m_total_count(0)
, m_height_scale(0)
, m_current_count(0.f)
, m_progress(0.f)
, m_progress_step(1.f)
, m_optimize_hard(false)
, m_progress_state(None)
{
}

void ProgressManager::setImagesCount(unsigned int images_count)
{
    m_images_count = images_count;
    m_total_count += m_states_count * images_count;
}

void ProgressManager::setOnProgress(std::function<void(int)> on_progress)
{
    m_on_progress = on_progress;
}

void ProgressManager::setProgressState(ProgressState progress_state)
{
    m_progress_state = progress_state;

    if (m_progress_state == Finished && m_on_progress)
        m_on_progress(100);
}

void ProgressManager::applyOptions(const GifOptions& options)
{
    unsigned int states_count = 3;
    if (options.scale_x > 1 || options.scale_y > 1)
        states_count++;

    if (options.optimize > 1)
        m_optimize_hard = true;

    m_states_count = states_count;
    m_height_scale = options.scale_y;
}

void ProgressManager::onProgress(int index)
{
    m_current_count += m_progress_step;
    const auto percent = m_current_count / static_cast<float>(m_total_count) * 100.f;

    if (round(percent - m_progress) >= 1.f && percent <= 100.f && m_on_progress)
    {
        m_progress = percent;
        m_on_progress(round(m_progress));
    }
}

void ProgressManager::adjustProgress(unsigned int images_count)
{
    if (images_count == m_images_count)
        return;

    m_total_count -= m_images_count;
    m_total_count += images_count;

    m_current_count = m_progress * static_cast<float>(m_total_count) / 100.f;
    m_progress_step = static_cast<float>(m_images_count) / static_cast<float>(images_count);
    m_images_count = images_count;
}

void ProgressManager::addImageHeight(unsigned int image_height)
{
    if (image_height == 1)
        m_total_count++;
    else
    {
        m_total_count += image_height * m_height_scale;
    }
}

bool ProgressManager::isOptimizeHard() const
{
    return m_optimize_hard;
}

ProgressManager& ProgressManager::instance()
{
    static ProgressManager progress_manager;
    return progress_manager;
}

void on_progress(int index)
{
    ProgressManager::instance().onProgress(index);
}

void set_images_count(unsigned int images_count)
{
    ProgressManager::instance().setImagesCount(images_count);
}

void set_progress_state(ProgressState progress_state)
{
    ProgressManager::instance().setProgressState(progress_state);
}

void adjust_progress(unsigned int images_count)
{
    ProgressManager::instance().adjustProgress(images_count);
}

void add_image_height(unsigned int image_height)
{
    ProgressManager::instance().addImageHeight(image_height);
}

int optimize_hard()
{
    return ProgressManager::instance().isOptimizeHard() ? 1 : 0;
}
