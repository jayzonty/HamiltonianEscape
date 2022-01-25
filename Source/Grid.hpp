#pragma once

#include <cstdint>
#include <vector>

/**
 * Grid
 */
 template <typename T>
class Grid
{
private:
    /**
     * Grid width
     */
    int32_t m_width;

    /**
     * Grid height
     */
    int32_t m_height;

    /**
     * Underlying buffer containing the data for the
     * cells in the grid
     */
    std::vector<T> m_buffer;

public:
    /**
     * @brief Constructor
     */
    Grid()
        : m_width(0)
        , m_height(0)
        , m_buffer()
    {
    }

    /**
     * @brief Destructor
     */
    ~Grid()
    {
    }

    /**
     * @brief Gets the width of the grid.
     * @return Grid width
     */
    int32_t GetWidth() const
    {
        return m_width;
    }

    /**
     * @brief Gets the height of the grid.
     * @return Grid height
     */
    int32_t GetHeight() const
    {
        return m_height;
    }

    /**
     * @brief Resizes the grid to the specified width and height.
     * @param[in] width New width
     * @param[in] height New height
     */
    void Resize(const int32_t& width, const int32_t& height)
    {
        m_width = width;
        m_height = height;
        m_buffer.resize(width * height);
    }

    /**
     * @brief Checks whether the specified location is a valid location or not.
     * @param[in] x X-coordinate
     * @param[in] y Y-coordinate
     * @return Returns true if the specified location is valid. Returns false otherwise.
     */
    bool IsValidLocation(const int32_t& x, const int32_t& y)
    {
        return (0 <= x) && (x < m_width) && (0 <= y) && (y < m_height);
    }

    /**
     * @brief Gets the pointer to the cell data at the specified location.
     * @param[in] x X-coordinate
     * @param[in] y Y-coordinate
     * @return Pointer to the cell data at the specified location. Returns nullptr if the location is invalid.
     */
    T* Get(const int32_t& x, const int32_t& y)
    {
        if (IsValidLocation(x, y))
        {
            int32_t flattenedIndex = y * m_width + x;
            return &m_buffer[flattenedIndex];
        }

        return nullptr;
    }

    /**
     * @brief Sets the cell data at the specified location.
     * @param[in] x X-coordinate
     * @param[in] y Y-coordinate
     * @param[in] data Cell data
     */
    void Set(const int32_t& x, const int32_t& y, const T& data)
    {
        if (IsValidLocation(x, y))
        {
            int32_t flattenedIndex = y * m_width + x;
            m_buffer[flattenedIndex] = data;
        }
    }
};
