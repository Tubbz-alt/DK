/*
    This file is part of DigitalKalmar(Кальмар-SDR)

    DigitalKalmar is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DigitalKalmar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with DigitalKalmar.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef RPU_KALIBRATOR_H
#define RPU_KALIBRATOR_H

#include <rpu_tract.h>

class RPU_kalibrator
{

private:
    LPT_ADDRESS    lpt;              // адрес порта

    MOD_SIGNAL     signal_type;      // синусоида / радиоимпульс
    int            modulation_freq;  // частота модуляции (только для радиоимпулься)
    int            att_idx;          // индекс значения аттенюации
    KALIB_TYPE     exit_type;        // внешний / внутренний
    STATUS         work_status;      // вкл / выкл

public:
    RPU_kalibrator();
    ~RPU_kalibrator();

    // функция для отправки команд через LPT
    lpOutc         gfpOut_char;
    void           send_code(unsigned short addr, unsigned char code);

/// ИНТЕРФЕЙС
    // изменение типа калибровочного сигнала
    void set_signal_type(MOD_SIGNAL new_signal_type);
    MOD_SIGNAL get_signal_type();

    // изменение частоты модуляции калибровочного сигнала
    void set_modulation_freq(int new_modulation_freq_in_kHz);
    int get_modulation_freq();

    // изменение аттенюации
    void set_att_idx(int new_att_idx);
    int get_att_idx();

    // смена калибратора
    void set_exit_type(KALIB_TYPE new_exit_type);
    KALIB_TYPE get_exit_type();

    // включение/выключение калибратора
    void set_work_status(STATUS new_work_status);
    STATUS get_work_status();

    // смена адреса LPT порта
    void set_lpt(LPT_ADDRESS new_lpt);
    LPT_ADDRESS get_lpt();

    RPU_kalibrator &operator=(RPU_kalibrator& RightKalib);

/// КОДОГРАММЫ
    // установка в РПУ частоты модуляции калибратора
    void set_modulation_freq_to_RPU();

    // установка в РПУ уровня аттенюации калибратора
    void set_attenuation_to_RPU();

    // включение / отключение калибратора в РПУ
    void set_work_status_to_RPU();

    // установка типа калибровочного сигнала в РПУ
    void set_signal_type_to_RPU(NUMERALS SupportTract);
};

#endif // RPU_KALIBRATOR_H
