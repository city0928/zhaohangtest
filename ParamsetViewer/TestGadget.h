#pragma once

#include <QMetaObject>

#include "yas/serialize.hpp"
#include "yas/std_types.hpp"

class TestGadget
{
    Q_GADGET
    Q_PROPERTY(int Idx READ getIdx WRITE setIdx)
  public:
    TestGadget(const int i){
        m_Idx = i;
    }

    int getIdx(){
        return m_Idx;
    }
    void setIdx(const int newi){
        m_Idx = newi;
    }

  private:
    int m_Idx{0};
    int m_Int{0};
};


class CustomType
{
    using Type = int;
    Q_GADGET
  public:
    // CustomType() = default;
    explicit CustomType(const Type& value, const Type& minValue, const Type& maxValue) noexcept
        : m_Vaule{value}, m_MinVaule{minValue}, m_MaxVaule{maxValue}
    {}
    CustomType(const CustomType& other) noexcept
        : m_Vaule{other.m_Vaule}, m_MinVaule{other.m_MinVaule},m_MaxVaule{other.m_MaxVaule}
    {}
    CustomType(CustomType&& other) noexcept
        : m_Vaule{std::move(other.m_Vaule)}, m_MinVaule{std::move(other.m_MinVaule)},m_MaxVaule{std::move(other.m_MaxVaule)}
    {}
    bool operator==(const CustomType& other) const
    {
        return this->m_Vaule == other.m_Vaule && this->m_MinVaule == other.m_MinVaule && this->m_MaxVaule == other.m_MaxVaule;
    }
    CustomType &operator=(const CustomType& other) noexcept {
        if(this != &other){
            this->m_Vaule = other.m_Vaule;
            this->m_MinVaule = other.m_MinVaule;
            this->m_MaxVaule = other.m_MaxVaule;
        }
        return *this;
    }
    CustomType &operator=(CustomType&& other) noexcept {
        if(this != &other){
            m_Vaule = std::move(other.m_Vaule);
            m_MinVaule = std::move(other.m_MinVaule);
            m_MaxVaule = std::move(other.m_MaxVaule);
        }
        return *this;
    }

  public:
    void setValue(const Type& value){
        m_Vaule = value;
    }
    void setMinValue(const Type& value){
        m_MinVaule = value;
    }
    void setMaxValue(const Type& value){
        m_MaxVaule = value;
    }
    Type getValue() const{
        return m_Vaule;
    }

  public:
    Type m_Vaule;
    Type m_MinVaule;
    Type m_MaxVaule;

  // public:
  //   YAS_DEFINE_STRUCT_SERIALIZE_NVP("",
  //                                   ("Vaule", m_Vaule),
  //                                   ("MinVaule", m_MinVaule),
  //                                   ("MaxVaule", m_MaxVaule));
};


class cTestBool
{
    Q_GADGET
    Q_PROPERTY(bool Value MEMBER m_Value READ getValue WRITE setValue)
    // Q_PROPERTY(bool DefaultValue MEMBER m_DefaultValue READ getDefaultValue WRITE setDefaultValue)
    // Q_PROPERTY(bool AlignmentVaribleInvaildValue MEMBER m_AlignmentVaribleInvaildValue READ getAlignmentVaribleInvaildValue WRITE setAlignmentVaribleInvaildValue)
  public:
    // cTestBool()
    //     : m_AlignmentVaribleInvaildValue{false} {
    // }
    explicit cTestBool(const bool& value) noexcept{}
    explicit cTestBool(const bool& value, const bool& defaultValue) noexcept{}
    bool operator==(const cTestBool& other) const{return true;}
  public:
    void setValue(const bool& value){}
    void setDefaultValue(const bool& value){}
    void setAlignmentVaribleInvaildValue(const bool& value){}
    bool getValue() const{
        return m_Value;
    }
    // bool getDefaultValue() const;
    // bool getAlignmentVaribleInvaildValue() const;
  private:
    bool m_Value;
    bool m_DefaultValue;
    bool m_AlignmentVaribleInvaildValue;
#ifndef Q_MOC_RUN
  public:
    YAS_DEFINE_STRUCT_SERIALIZE_NVP("",
                                    ("Value", m_Value),
                                    ("DefaultValue", m_DefaultValue),
                                    ("AlignmentVaribleInvaildValue", m_AlignmentVaribleInvaildValue));
#endif
};
