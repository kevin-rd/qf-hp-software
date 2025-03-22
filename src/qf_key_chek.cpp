#include "qf_key_chek.h"
#include "User.h"
#include "user_datas.h"

void get_mac(uint8_t *mac_ret)
{
    uint64_t chipid = ESP.getChipId();
    uint8_t *mac = (uint8_t *)(&chipid);
    mac[3] = mac[0] + mac[1];
    mac[4] = mac[0] + mac[2];
    mac[5] = mac[1] + mac[2];
    memcpy(mac_ret, mac, 6);
}

void get_sn(uint8_t *sn_ret)
{
    memcpy(sn_ret, user_datas.key, 6);
}

// 函数qf_pass_key_get用于获取密钥
void qf_pass_key_get(uint8_t *mac, uint8_t *key, size_t bytes)
{
    uint8_t mac_tmp[6];
    uint8_t key_tmp[6];
    uint8_t my_key[2] = {qf_private_key};
    memcpy(key_tmp, mac, 6);
    memcpy(mac_tmp, mac, 6);

    for (size_t i = 0; i < 6; i++)
        mac_tmp[i] ^= my_key[0];

    for (size_t i = 0; i < 6; i++)
        key_tmp[i] ^= my_key[1];

    mac_tmp[0] += key_tmp[5];
    mac_tmp[1] += key_tmp[0];
    mac_tmp[2] += key_tmp[1];
    mac_tmp[3] += key_tmp[2];
    mac_tmp[4] += key_tmp[3];
    mac_tmp[5] += key_tmp[4];

    mac_tmp[1] += mac_tmp[0];
    mac_tmp[2] += mac_tmp[1];
    mac_tmp[3] += mac_tmp[2];
    mac_tmp[4] += mac_tmp[3];
    mac_tmp[5] += mac_tmp[4];
    mac_tmp[0] += mac_tmp[5];

    key_tmp[1] += mac_tmp[0];
    key_tmp[2] += mac_tmp[1];
    key_tmp[3] += mac_tmp[2];
    key_tmp[4] += mac_tmp[3];
    key_tmp[5] += mac_tmp[4];
    key_tmp[0] += mac_tmp[5];

    for (size_t i = 0; i < 6; i++)
        mac_tmp[i] ^= key_tmp[i];

    memcpy(key_tmp, mac_tmp, 6);

    for (size_t i = 0; i < 5; i++)
        mac_tmp[i] ^= key_tmp[i + 1];
    mac_tmp[5] ^= key_tmp[0];

    if (bytes != 3)
    {
        memcpy(key, mac_tmp, 6);
        return;
    }

    key_tmp[0] = mac_tmp[0] ^ mac_tmp[3];
    key_tmp[1] = mac_tmp[1] ^ mac_tmp[4];
    key_tmp[2] = mac_tmp[2] ^ mac_tmp[5];

    memcpy(key, key_tmp, 3);
}