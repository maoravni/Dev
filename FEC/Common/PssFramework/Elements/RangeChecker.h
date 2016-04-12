/*
 * RangeChecker.h
 *
 *  Created on: 11 Jun 2013
 *      Author: maora
 */

#ifndef RANGECHECKER_H_
#define RANGECHECKER_H_

template <class _type>
class RangeChecker
{
private:
    _type m_minValue;
    _type m_maxValue;
    bool m_checkMin;
    bool m_checkMax;
public:
    _type getMin() {return m_minValue;}
    _type getMax() {return m_maxValue;}
    RangeChecker();
    void updateRange(_type min, _type max, bool checkMin, bool checkMax);
    bool inRange(_type value);
    bool aboveMaxRange(_type value);
    bool belowMinRange(_type value);
    _type clampToRange(_type value);

    template <class T> friend class Serializer;

};

template<class _type>
inline bool RangeChecker<_type>::inRange(_type value)
{
//    if (m_minValue == 0 && m_maxValue == 0)
//        return true;
//    if (m_checkMin == false && m_checkMax == false)
//        return true;
//    if (value >= m_minValue && value <= m_maxValue)
//        return true;
    if (aboveMaxRange(value) || belowMinRange(value))
        return false;
    return true;
}

template<class _type>
inline RangeChecker<_type>::RangeChecker()
{
    m_minValue = 0;
    m_maxValue = 0;
    m_checkMin = false;
    m_checkMax = false;
}

template<class _type>
inline void RangeChecker<_type>::updateRange(_type min, _type max, bool checkMin, bool checkMax)
{
    m_minValue = min;
    m_maxValue = max;
    m_checkMin = checkMin;
    m_checkMax = checkMax;
}

template<class _type>
inline bool RangeChecker<_type>::aboveMaxRange(_type value)
{
//    if (m_maxValue == 0)
//        return false;
//    if (m_checkMax == false)
//        return false;
    return (m_checkMax && value >= m_maxValue);
}

template<class _type>
inline bool RangeChecker<_type>::belowMinRange(_type value)
{
//    if (m_minValue == 0)
//        return true;
//    if (m_checkMin == false)
//        return false;
    return (m_checkMin && value < m_minValue);
}

template<class _type>
inline _type RangeChecker<_type>::clampToRange(_type value)
{
    if (m_checkMin == false && m_checkMax == false)
        return value;
    if (value < m_minValue)
        return m_minValue;
    if (value > m_maxValue)
        return m_maxValue;
    return value;
}

#endif /* RANGECHECKER_H_ */
