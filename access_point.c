#include "access_point.h"

SETTINGS *p_settings;

char html_page[HTML_RESULT_SIZE];

char in1_midi_ch_html_str[265];
char in1_base_note_html_str[265];
char in2_midi_ch_html_str[265];
char in2_base_note_html_str[265];
char an_midi_ch_html_str[265];

void update_html_page() {
    // clear html_page buffer
    memset(html_page, '\0', HTML_RESULT_SIZE);

    strcat(html_page, "\
    <!DOCTYPE html>\
    <html>\
        <head>\
            <style>\
                .form-style{\
                    width:480px;\
                    padding:30px;\
                    margin:40px auto;\
                    background: #FFF;\
                    border-radius: 10px;\
                    -webkit-border-radius:10px;\
                    -moz-border-radius: 10px;\
                    box-shadow: 0px 0px 10px rgba(0, 0, 0, 0.13);\
                    -moz-box-shadow: 0px 0px 10px rgba(0, 0, 0, 0.13);\
                    -webkit-box-shadow: 0px 0px 10px rgba(0, 0, 0, 0.13);\
                }\
                .form-style .inner-wrap{\
                    padding: 20px;\
                    background: #F8F8F8;\
                    border-radius: 6px;\
                    margin-bottom: 15px;\
                }\
                .form-style h1{\
                    background: #2A88AD;\
                    padding: 20px 30px 15px 30px;\
                    margin: -30px -30px 30px -30px;\
                    border-radius: 10px 10px 0 0;\
                    -webkit-border-radius: 10px 10px 0 0;\
                    -moz-border-radius: 10px 10px 0 0;\
                    color: #fff;\
                    text-shadow: 1px 1px 3px rgba(0, 0, 0, 0.12);\
                    font: normal 30px 'Bitter', serif;\
                    -moz-box-shadow: inset 0px 2px 2px 0px rgba(255, 255, 255, 0.17);\
                    -webkit-box-shadow: inset 0px 2px 2px 0px rgba(255, 255, 255, 0.17);\
                    box-shadow: inset 0px 2px 2px 0px rgba(255, 255, 255, 0.17);\
                    border: 1px solid #257C9E;\
                }\
                .form-style h1 > span{\
                    display: block;\
                    margin-top: 2px;\
                    font: 13px Arial, Helvetica, sans-serif;\
                }\
                .form-style label{\
                    display: block;\
                    font: 13px Arial, Helvetica, sans-serif;\
                    color: #888;\
                    margin-bottom: 15px;\
                }\
                .form-style input[type=\"number\"],\
                .form-style select {\
                    display: block;\
                    box-sizing: border-box;\
                    -webkit-box-sizing: border-box;\
                    -moz-box-sizing: border-box;\
                    width: 100%%;\
                    padding: 8px;\
                    border-radius: 6px;\
                    -webkit-border-radius:6px;\
                    -moz-border-radius:6px;\
                    border: 2px solid #fff;\
                    box-shadow: inset 0px 1px 1px rgba(0, 0, 0, 0.33);\
                    -moz-box-shadow: inset 0px 1px 1px rgba(0, 0, 0, 0.33);\
                    -webkit-box-shadow: inset 0px 1px 1px rgba(0, 0, 0, 0.33);\
                }\
                .form-style .section{\
                    font: normal 20px 'Bitter', serif;\
                    color: #2A88AD;\
                    margin-bottom: 5px;\
                }\
                .form-style input[type=\"submit\"],\
                .form-style input[type=\"button\"]{\
                    background: #2A88AD;\
                    padding: 8px 20px 8px 20px;\
                    border-radius: 5px;\
                    -webkit-border-radius: 5px;\
                    -moz-border-radius: 5px;\
                    color: #fff;\
                    text-shadow: 1px 1px 3px rgba(0, 0, 0, 0.12);\
                    font: normal 30px 'Bitter', serif;\
                    -moz-box-shadow: inset 0px 2px 2px 0px rgba(255, 255, 255, 0.17);\
                    -webkit-box-shadow: inset 0px 2px 2px 0px rgba(255, 255, 255, 0.17);\
                    box-shadow: inset 0px 2px 2px 0px rgba(255, 255, 255, 0.17);\
                    border: 1px solid #257C9E;\
                    font-size: 15px;\
                }\
                .form-style input[type=\"button\"]{\
                    float: right;\
                }\
                .form-style input[type=\"submit\"]:hover,\
                .form-style input[type=\"button\"]:hover{\
                    background: #2A6881;\
                    -moz-box-shadow: inset 0px 2px 2px 0px rgba(255, 255, 255, 0.28);\
                    -webkit-box-shadow: inset 0px 2px 2px 0px rgba(255, 255, 255, 0.28);\
                    box-shadow: inset 0px 2px 2px 0px rgba(255, 255, 255, 0.28);\
                }\
            </style>\
        </head>\
        <body>\
            <div class=\"form-style\">"
    );

    // Title
    strcat(html_page, "\
        <h1>"
    );
    strcat(html_page, DEV_NAME);
    strcat(html_page, "<span>Firmware version: ");
    strcat(html_page, FW_VERSION);
    strcat(html_page, "</span></h1>\
            <form>\
                <div class=\"section\">General</div>"
    );

    //MIDI bus rate
    if (p_settings->fast_midi == 1) {
        strcat(html_page, "\
            <div class=\"inner-wrap\"><label>\
                MIDI serial bus rate\
                <select name=\"fast_midi\">\
                    <option value=\"0\" > MIDI-compliant rate </option>\
                    <option value=\"1\" selected> High speed (not compatible with third-party MIDI equipment) </option>\
                </select>\
            </label></div>"
        );
    }
    else {
        strcat(html_page, "\
            <div class=\"inner-wrap\"><label>\
                MIDI serial bus rate\
                <select name=\"fast_midi\">\
                    <option value=\"0\" selected> MIDI-compliant rate </option>\
                    <option value=\"1\"> High speed (not compatible with third-party MIDI equipment) </option>\
                </select>\
            </label></div>"
        );
    }

    // header1
    strcat(html_page, "\
        <div class=\"section\">Input header 1</div>\
        <div class=\"inner-wrap\">"
    );

    sprintf(in1_midi_ch_html_str, "<label> Midi channel <input type=\"number\" name=\"in1_m_ch\" min=\"1\" max=\"16\" value=%d> </label>", p_settings->in1_m_ch+1);
    strcat(html_page, in1_midi_ch_html_str);

    // message type
    if (p_settings->in1_m_type == 0) {
        strcat(html_page, "\
            <label>\
                Message type\
                <select name=\"in1_m_type\">\
                    <option value=\"0\" selected> NOTE ON / NOTE OFF </option>\
                    <option value=\"1\"> PROGRAM CHANGE </option>\
                </select>\
            </label>"
        );
    }
    else {
        strcat(html_page, "\
            <label>\
                Message type\
                <select name=\"in1_m_type\">\
                    <option value=\"0\"> NOTE ON / NOTE OFF </option>\
                    <option value=\"1\" selected> PROGRAM CHANGE </option>\
                </select>\
            </label>"
        );
    }

    sprintf(in1_base_note_html_str, "<label> Base MIDI number (first input's MIDI note or ProgCh number) <input type=\"number\" name=\"in1_base_m\" min=\"0\" max=\"119\" value=%d> </label>", p_settings->in1_base_m);
    strcat(html_page, in1_base_note_html_str);

    // inverting
    if (p_settings->in1_inv == 1) {
        strcat(html_page, "\
            <label>\
                Invert inputs\
                <select name=\"in1_inv\">\
                    <option value=\"0\"> NOTE ON (ProgChange) when an input is activated </option>\
                    <option value=\"1\" selected> NOTE ON (ProgChange) when an input is deactivated </option>\
                </select>\
            </label></div>"
        );
    }
    else {
        strcat(html_page, "\
            <label>\
                Invert inputs\
                <select name=\"in1_inv\">\
                    <option value=\"0\" selected> NOTE ON (ProgChange) when an input is activated </option>\
                    <option value=\"1\"> NOTE ON (ProgChange) when an input is deactivated </option>\
                </select>\
            </label></div>"
        );
    }

    // header2
    strcat(html_page, "\
        <div class=\"section\">Input header 2</div>\
        <div class=\"inner-wrap\">"
    );

    sprintf(in2_midi_ch_html_str, "<label> Midi channel <input type=\"number\" name=\"in2_m_ch\" min=\"1\" max=\"16\" value=%d> </label>", p_settings->in2_m_ch+1);
    strcat(html_page, in2_midi_ch_html_str);

    // message type
    if (p_settings->in2_m_type == 0) {
        strcat(html_page, "\
            <label>\
                Message type\
                <select name=\"in2_m_type\">\
                    <option value=\"0\" selected> NOTE ON / NOTE OFF </option>\
                    <option value=\"1\"> PROGRAM CHANGE </option>\
                </select>\
            </label>"
        );
    }
    else {
        strcat(html_page, "\
            <label>\
                Message type\
                <select name=\"in2_m_type\">\
                    <option value=\"0\"> NOTE ON / NOTE OFF </option>\
                    <option value=\"1\" selected> PROGRAM CHANGE </option>\
                </select>\
            </label>"
        );
    }

    sprintf(in2_base_note_html_str, "<label>Base MIDI number (first input's MIDI note or ProgCh number) <input type=\"number\" name=\"in2_base_m\" min=\"0\" max=\"119\" value=%d> </label>", p_settings->in2_base_m);
    strcat(html_page, in2_base_note_html_str);

    // inverting
    if (p_settings->in2_inv == 1) {
        strcat(html_page, "\
            <label>\
                Invert inputs\
                <select name=\"in2_inv\">\
                    <option value=\"0\"> NOTE ON (ProgChange) when an input is activated </option>\
                    <option value=\"1\" selected> NOTE ON (ProgChange) when an input is deactivated </option>\
                </select>\
            </label></div>"
        );
    }
    else {
        strcat(html_page, "\
            <label>\
                Invert inputs\
                <select name=\"in2_inv\">\
                    <option value=\"0\" selected> NOTE ON (ProgChange) when an input is activated </option>\
                    <option value=\"1\"> NOTE ON (ProgChange) when an input is deactivated </option>\
                </select>\
            </label></div>"
        );
    }

    strcat(html_page, "\
        <div class=\"button-section\">\
            <input type=\"submit\" value=\"Save settings\">\
            <a href=\"?default=1\">\
                <input type=\"button\" value=\"Set default values\" />\
            </a>\
        </span></div>\
    </html>");

    strcat(html_page, "\
                </form>\
            </div>\
        </body>\
    </html>");
}

static err_t tcp_close_client_connection(TCP_CONNECT_STATE_T *con_state, struct tcp_pcb *client_pcb, err_t close_err) {
    if (client_pcb) {
        assert(con_state && con_state->pcb == client_pcb);
        tcp_arg(client_pcb, NULL);
        tcp_poll(client_pcb, NULL, 0);
        tcp_sent(client_pcb, NULL);
        tcp_recv(client_pcb, NULL);
        tcp_err(client_pcb, NULL);
        err_t err = tcp_close(client_pcb);
        if (err != ERR_OK) {
            tcp_abort(client_pcb);
            close_err = ERR_ABRT;
        }
        if (con_state) {
            free(con_state);
        }
    }
    return close_err;
}

static err_t tcp_server_sent(void *arg, struct tcp_pcb *pcb, u16_t len) {
    TCP_CONNECT_STATE_T *con_state = (TCP_CONNECT_STATE_T*)arg;
    con_state->sent_len += len;
    if (con_state->sent_len >= con_state->header_len + con_state->result_len) {
        return tcp_close_client_connection(con_state, pcb, ERR_OK);
    }
    return ERR_OK;
}

static int test_server_content(const char *request, const char *params, char *result, size_t max_result_len) {
    int len = 0;
    if (strncmp(request, SET_URL_SEGMENT, sizeof(SET_URL_SEGMENT) - 1) == 0) {
        // See if the user changed it
        if (params) {
            int fast_midi_param;
            int in1_m_ch;
            int in1_m_type;
            int in1_inv;
            int in1_base_m;
            int in2_m_ch;
            int in2_m_type;
            int in2_inv;
            int in2_base_m;

            int no_params = sscanf(params, "fast_midi=%d&in1_m_ch=%d&in1_m_type=%d&in1_base_m=%d&in1_inv=%d&in2_m_ch=%d&in2_m_type=%d&in2_base_m=%d&in2_inv=%d",\
                &fast_midi_param, &in1_m_ch, &in1_m_type, &in1_base_m, &in1_inv, &in2_m_ch, &in2_m_type, &in2_base_m, &in2_inv);

            if (no_params == 9) {
                if (fast_midi_param>=0 && fast_midi_param<=1) p_settings->fast_midi = (uint8_t)fast_midi_param;
                if (in1_m_ch>=1 && in1_m_ch<=16) p_settings->in1_m_ch = (uint8_t)in1_m_ch-1;
                if (in1_m_type>=0 && in1_m_type<=1) p_settings->in1_m_type = (uint8_t)in1_m_type;
                if (in1_inv>=0 && in1_inv<=1) p_settings->in1_inv = (uint8_t)in1_inv;
                if (in1_base_m>=0 && in1_base_m<=119) p_settings->in1_base_m = (uint8_t)in1_base_m;
                if (in2_m_ch>=1 && in2_m_ch<=16) p_settings->in2_m_ch = (uint8_t)in2_m_ch-1;
                if (in2_m_type>=0 && in2_m_type<=1) p_settings->in2_m_type = (uint8_t)in2_m_type;
                if (in2_inv>=0 && in2_inv<=1) p_settings->in2_inv = (uint8_t)in2_inv;
                if (in2_base_m>=0 && in2_base_m<=119) p_settings->in2_base_m = (uint8_t)in2_base_m;

                save_settings(p_settings);
            }

            // default settings
            if (strstr(params, "default=1") != NULL) {
                p_settings->fast_midi = FAST_MIDI_DEF;
                p_settings->in1_m_ch = IN1_M_CH_DEF;
                p_settings->in1_m_type = IN1_M_TYPE_DEF;
                p_settings->in1_inv = IN1_INV_DEF;
                p_settings->in1_base_m = IN1_BASE_M_DEF;
                p_settings->in2_m_ch = IN2_M_CH_DEF;
                p_settings->in2_m_type = IN2_M_TYPE_DEF;
                p_settings->in2_inv = IN2_INV_DEF;
                p_settings->in2_base_m = IN2_BASE_M_DEF;

                save_settings(p_settings);
            }            
        }
        // Generate result
        update_html_page();
        len = snprintf(result, max_result_len, html_page);
    }
    return len;
}

err_t tcp_server_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err) {
    TCP_CONNECT_STATE_T *con_state = (TCP_CONNECT_STATE_T*)arg;
    if (!p) {
        return tcp_close_client_connection(con_state, pcb, ERR_OK);
    }
    assert(con_state && con_state->pcb == pcb);
    if (p->tot_len > 0) {

        // Copy the request into the buffer
        pbuf_copy_partial(p, con_state->headers, p->tot_len > sizeof(con_state->headers) - 1 ? sizeof(con_state->headers) - 1 : p->tot_len, 0);

        // Handle GET request
        if (strncmp(HTTP_GET, con_state->headers, sizeof(HTTP_GET) - 1) == 0) {
            char *request = con_state->headers + sizeof(HTTP_GET); // + space
            char *params = strchr(request, '?');
            if (params) {
                if (*params) {
                    char *space = strchr(request, ' ');
                    *params++ = 0;
                    if (space) {
                        *space = 0;
                    }
                } else {
                    params = NULL;
                }
            }

            // Generate content
            con_state->result_len = test_server_content(request, params, con_state->result, sizeof(con_state->result));

            // Check we had enough buffer space
            if (con_state->result_len > sizeof(con_state->result) - 1) {
                return tcp_close_client_connection(con_state, pcb, ERR_CLSD);
            }

            // Generate web page
            if (con_state->result_len > 0) {
                con_state->header_len = snprintf(con_state->headers, sizeof(con_state->headers), HTTP_RESPONSE_HEADERS,
                    200, con_state->result_len);
                if (con_state->header_len > sizeof(con_state->headers) - 1) {
                    return tcp_close_client_connection(con_state, pcb, ERR_CLSD);
                }
            } else {
                // Send redirect
                con_state->header_len = snprintf(con_state->headers, sizeof(con_state->headers), HTTP_RESPONSE_REDIRECT,
                    ipaddr_ntoa(con_state->gw));
            }

            // Send the headers to the client
            con_state->sent_len = 0;
            err_t err = tcp_write(pcb, con_state->headers, con_state->header_len, 0);
            if (err != ERR_OK) {
                return tcp_close_client_connection(con_state, pcb, err);
            }

            // Send the body to the client
            if (con_state->result_len) {
                err = tcp_write(pcb, con_state->result, con_state->result_len, 0);
                if (err != ERR_OK) {
                    return tcp_close_client_connection(con_state, pcb, err);
                }
            }
        }
        tcp_recved(pcb, p->tot_len);
    }
    pbuf_free(p);
    return ERR_OK;
}

static err_t tcp_server_poll(void *arg, struct tcp_pcb *pcb) {
    TCP_CONNECT_STATE_T *con_state = (TCP_CONNECT_STATE_T*)arg;
    return tcp_close_client_connection(con_state, pcb, ERR_OK); // Just disconnect clent?
}

static void tcp_server_err(void *arg, err_t err) {
    TCP_CONNECT_STATE_T *con_state = (TCP_CONNECT_STATE_T*)arg;
    if (err != ERR_ABRT) {
        tcp_close_client_connection(con_state, con_state->pcb, err);
    }
}

static err_t tcp_server_accept(void *arg, struct tcp_pcb *client_pcb, err_t err) {
    TCP_SERVER_T *state = (TCP_SERVER_T*)arg;
    if (err != ERR_OK || client_pcb == NULL) {
        return ERR_VAL;
    }

    // Create the state for the connection
    TCP_CONNECT_STATE_T *con_state = calloc(1, sizeof(TCP_CONNECT_STATE_T));
    if (!con_state) {
        return ERR_MEM;
    }
    con_state->pcb = client_pcb; // for checking
    con_state->gw = &state->gw;

    // setup connection to client
    tcp_arg(client_pcb, con_state);
    tcp_sent(client_pcb, tcp_server_sent);
    tcp_recv(client_pcb, tcp_server_recv);
    tcp_poll(client_pcb, tcp_server_poll, POLL_TIME_S * 2);
    tcp_err(client_pcb, tcp_server_err);

    return ERR_OK;
}

static bool tcp_server_open(void *arg, const char *ap_name) {
    TCP_SERVER_T *state = (TCP_SERVER_T*)arg;

    struct tcp_pcb *pcb = tcp_new_ip_type(IPADDR_TYPE_ANY);
    if (!pcb) {
        return false;
    }

    err_t err = tcp_bind(pcb, IP_ANY_TYPE, TCP_PORT);
    if (err) {
        return false;
    }

    state->server_pcb = tcp_listen_with_backlog(pcb, 1);
    if (!state->server_pcb) {
        if (pcb) {
            tcp_close(pcb);
        }
        return false;
    }

    tcp_arg(state->server_pcb, state);
    tcp_accept(state->server_pcb, tcp_server_accept);

    return true;
}

// -------------- external ------------
int wifi_ap_proc(SETTINGS *set) {
    p_settings = set;

    TCP_SERVER_T *state = calloc(1, sizeof(TCP_SERVER_T));
    if (!state) {
        return 1;
    }

    // light on the signal led - AP point
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);

    const char *password = NULL;

    cyw43_arch_enable_ap_mode(DEV_NAME, password, CYW43_AUTH_WPA2_AES_PSK);

    ip4_addr_t mask;
    IP4_ADDR(ip_2_ip4(&state->gw), 192, 168, 4, 1);
    IP4_ADDR(ip_2_ip4(&mask), 255, 255, 255, 0);

    // Start the dhcp server
    dhcp_server_t dhcp_server;
    dhcp_server_init(&dhcp_server, &state->gw, &mask);

    // Start the dns server
    dns_server_t dns_server;
    dns_server_init(&dns_server, &state->gw);

    if (!tcp_server_open(state, DEV_NAME)) {
        return 1;
    }

    while(1) {
        // if you are using pico_cyw43_arch_poll, then you must poll periodically from your
        // main loop (not from a timer interrupt) to check for Wi-Fi driver or lwIP work that needs to be done.
        cyw43_arch_poll();
        // you can poll as often as you like, however if you have nothing else to do you can
        // choose to sleep until either a specified time, or cyw43_arch_poll() has work to do:
        cyw43_arch_wait_for_work_until(make_timeout_time_ms(1000));
    }

    return 0;
}
