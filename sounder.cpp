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

#include "sounder.h"

SOUNDER::SOUNDER(SDR_params *new_sdr_params, DSP_params *new_dsp_params): sdr_params{new_sdr_params}, dsp_params{new_dsp_params}
{
    QAudioFormat format;

    format.setSampleRate(DSP_SOUND_SAMPLE_RATE);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if(info.isFormatSupported(format)){
        dsp_params->sound_params.audio_out = new QAudioOutput(format, this);
        dsp_params->sound_params.audio_device = dsp_params->sound_params.audio_out->start();
        if(dsp_params->sound_params.is_mute)
            dsp_params->sound_params.audio_out->setVolume(0);
        else
            dsp_params->sound_params.audio_out->setVolume(dsp_params->sound_params.sound_volume / 100.);
    }
}

SOUNDER::~SOUNDER()
{}

void SOUNDER::get_sound_step(Ipp32f *cell)
{
    // конвертация 32f -> 16s
    ippsConvert_32f16s_Sfs(cell,
                           dsp_params->sound_params.rb.get_current_cell(),
                           static_cast<int>(dsp_params->sound_params.rb.cell_size),
                           ippRndNear,
                           dsp_params->sound_params.scale_facor);

    if(dsp_params->sound_params.write_to_file)
        emit write_to_file16s(dsp_params->sound_params.rb.get_current_cell(),
                              static_cast<int>(dsp_params->sound_params.rb.cell_size));

    // на звуковую карту
    dsp_params->sound_params.audio_device->write(reinterpret_cast<char*>(dsp_params->sound_params.rb.get_current_cell()),
                                                  dsp_params->sound_params.rb.cell_size * sizeof(Ipp16s));

    // инкремент итератора по звуковому КБ
    dsp_params->sound_params.rb.increase_cnt();
}
