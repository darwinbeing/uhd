//
// Copyright 2010-2011 Ettus Research LLC
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef INCLUDED_USRP2_IMPL_HPP
#define INCLUDED_USRP2_IMPL_HPP

#include "usrp2_iface.hpp"
#include "clock_ctrl.hpp"
#include "codec_ctrl.hpp"
#include "rx_frontend_core_200.hpp"
#include "tx_frontend_core_200.hpp"
#include "rx_dsp_core_200.hpp"
#include "tx_dsp_core_200.hpp"
#include "time64_core_200.hpp"
#include <uhd/property_tree.hpp>
#include <uhd/usrp/gps_ctrl.hpp>
#include <uhd/device.hpp>
#include <uhd/utils/pimpl.hpp>
#include <uhd/types/dict.hpp>
#include <uhd/types/otw_type.hpp>
#include <uhd/types/stream_cmd.hpp>
#include <uhd/types/clock_config.hpp>
#include <uhd/usrp/dboard_eeprom.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <uhd/transport/vrt_if_packet.hpp>
#include <uhd/transport/udp_simple.hpp>
#include <uhd/transport/udp_zero_copy.hpp>
#include <uhd/usrp/dboard_manager.hpp>
#include <uhd/usrp/subdev_spec.hpp>

static const double mimo_clock_delay_usrp2_rev4 = 4.18e-9;
static const double mimo_clock_delay_usrp_n2xx = 3.55e-9;
static const size_t mimo_clock_sync_delay_cycles = 137;
static const size_t USRP2_SRAM_BYTES = size_t(1 << 20);

/*!
 * Make a usrp2 dboard interface.
 * \param iface the usrp2 interface object
 * \param clk_ctrl the clock control object
 * \return a sptr to a new dboard interface
 */
uhd::usrp::dboard_iface::sptr make_usrp2_dboard_iface(
    usrp2_iface::sptr iface,
    usrp2_clock_ctrl::sptr clk_ctrl
);

/*!
 * USRP2 implementation guts:
 * The implementation details are encapsulated here.
 * Handles device properties and streaming...
 */
class usrp2_impl{
public:
    usrp2_impl(const uhd::device_addr_t &);
    ~usrp2_impl(void);
    uhd::property_tree::sptr _tree;
private:
    struct mboard_stuff_type{
        usrp2_iface::sptr iface;
        usrp2_clock_ctrl::sptr clock;
        usrp2_codec_ctrl::sptr codec;
        gps_ctrl::sptr gps;
        rx_frontend_core_200::sptr rx_fe;
        tx_frontend_core_200::sptr tx_fe;
        std::vector<rx_dsp_core_200::sptr> rx_dsps;
        tx_dsp_core_200::sptr tx_dsp;
        time64_core_200::sptr time64;
        std::vector<uhd::transport::zero_copy_if::sptr> dsp_xports;
        std::vector<uhd::transport::zero_copy_if::sptr> err_xports;
        uhd::usrp::dboard_manager::sptr dboard_manager;
        uhd::usrp::dboard_iface::sptr dboard_iface;
    };
    std::vector<mboard_stuff_type> _mboard_stuff;

    void set_mb_eeprom(const size_t which_mb, const uhd::usrp::mboard_eeprom_t &mb_eeprom);
    void set_db_eeprom(const size_t which_mb, const std::string &type, const uhd::usrp::dboard_eeprom_t &db_eeprom);

    uhd::sensor_value_t get_mimo_locked(const size_t which_mb);
    uhd::sensor_value_t get_ref_locked(const size_t which_mb);

};

#endif /* INCLUDED_USRP2_IMPL_HPP */