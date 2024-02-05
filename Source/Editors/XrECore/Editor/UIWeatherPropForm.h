#pragma once

class UIWeatherPropForm: public XrUI
{
public:
    UIWeatherPropForm();
    virtual ~UIWeatherPropForm();
    virtual void Draw();

    static void  Update();
    static void  Show();

    float m_weather_time;
    float m_speed_time;

    bool m_sun_visible;
    bool m_use_sun_dir;
    bool m_use_hemi;
    void IsShowSunChanged() const;
    void IsUseSunDirChanged() const;
    void IsUseHemiChanged() const;

private:
    static UIWeatherPropForm* Form;
    ref_texture m_weather_properties;
};
