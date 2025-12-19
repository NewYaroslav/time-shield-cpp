#include <time_shield/config.hpp>

#if TIME_SHIELD_ENABLE_NTP_CLIENT

#include <time_shield/ntp_client_pool.hpp>

#include <cassert>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

using namespace time_shield;

struct FakeReply {
    bool    ok = true;
    int     error_code = 0;
    int     stratum = 1;
    int64_t offset_us = 0;
    int64_t delay_us = 0;
    bool    throws_on_query = false;
};

class FakeNtpClient {
public:
    FakeNtpClient(const std::string& host, int port)
        : m_host(host)
        , m_port(port) {
        (void)m_port;
    }

    bool query() {
        const FakeReply& reply = scenario()[m_host];
        if (reply.throws_on_query) {
            throw std::runtime_error("fake");
        }
        m_last_error = reply.error_code;
        return reply.ok;
    }

    int last_error_code() const { return m_last_error; }
    int64_t offset_us() const { return scenario()[m_host].offset_us; }
    int64_t delay_us() const { return scenario()[m_host].delay_us; }
    int stratum() const { return scenario()[m_host].stratum; }

    static void set_reply(const std::string& host, const FakeReply& reply) {
        scenario()[host] = reply;
    }

    static void clear() {
        scenario().clear();
    }

private:
    static std::unordered_map<std::string, FakeReply>& scenario() {
        static std::unordered_map<std::string, FakeReply> data;
        return data;
    }

    std::string m_host;
    int         m_port;
    int         m_last_error = 0;
};

using Pool = NtpClientPoolT<FakeNtpClient>;

static NtpSample make_sample(bool ok, int64_t offset, int64_t delay, int64_t max_delay) {
    NtpSample sample;
    sample.is_ok = ok;
    sample.offset_us = offset;
    sample.delay_us = delay;
    sample.max_delay_us = max_delay;
    return sample;
}

int main() {
    {
        NtpPoolConfig cfg;
        cfg.min_valid_samples = 3;
        cfg.aggregation = NtpPoolConfig::Aggregation::Median;
        cfg.smoothing_alpha = 1.0;
        Pool pool(cfg);

        std::vector<NtpSample> samples;
        samples.push_back(make_sample(true, 100, 0, 0));
        samples.push_back(make_sample(true, 200, 0, 0));
        samples.push_back(make_sample(true, 300, 0, 0));

        assert(pool.apply_samples(samples));
        assert(pool.offset_us() == 200);
    }

    {
        NtpPoolConfig cfg;
        cfg.min_valid_samples = 2;
        cfg.aggregation = NtpPoolConfig::Aggregation::MedianMadTrim;
        cfg.smoothing_alpha = 1.0;
        Pool pool(cfg);

        std::vector<NtpSample> samples;
        samples.push_back(make_sample(true, 100, 0, 0));
        samples.push_back(make_sample(true, 105, 0, 0));
        samples.push_back(make_sample(true, 100000, 0, 0));

        assert(pool.apply_samples(samples));
        assert(pool.offset_us() == 102);
    }

    {
        NtpPoolConfig cfg;
        cfg.min_valid_samples = 1;
        cfg.aggregation = NtpPoolConfig::Aggregation::BestDelay;
        Pool pool(cfg);

        std::vector<NtpSample> samples;
        samples.push_back(make_sample(true, 10, 100000, 200000));
        samples.push_back(make_sample(true, 5, 300000, 200000));

        assert(pool.apply_samples(samples));
        assert(pool.offset_us() == 10);

        samples.clear();
        samples.push_back(make_sample(true, 40, 50000, 200000));
        samples.push_back(make_sample(true, 20, 30000, 200000));
        assert(pool.apply_samples(samples));
        assert(pool.offset_us() == 20);
    }

    {
        NtpPoolConfig cfg;
        cfg.min_valid_samples = 1;
        cfg.smoothing_alpha = 2.0;
        Pool pool(cfg);

        std::vector<NtpSample> samples;
        samples.push_back(make_sample(true, 500, 0, 0));
        assert(pool.apply_samples(samples));
        assert(pool.offset_us() == 500);
    }

    {
        NtpPoolConfig cfg;
        cfg.min_valid_samples = 1;
        cfg.smoothing_alpha = -0.5;
        Pool pool(cfg);

        std::vector<NtpSample> samples;
        samples.push_back(make_sample(true, 700, 0, 0));
        assert(pool.apply_samples(samples));
        assert(pool.offset_us() == 0);
    }

    {
        NtpPoolConfig cfg;
        cfg.min_valid_samples = 2;
        cfg.smoothing_alpha = 1.0;
        Pool pool(cfg);

        std::vector<NtpSample> samples;
        samples.push_back(make_sample(true, 123, 0, 0));
        assert(!pool.apply_samples(samples));
        assert(pool.offset_us() == 0);
    }

    {
        FakeNtpClient::clear();
        FakeReply a;
        a.offset_us = 100;
        a.delay_us = 10;
        FakeReply b;
        b.offset_us = 200;
        b.delay_us = 20;
        FakeReply c;
        c.offset_us = 300;
        c.delay_us = 30;
        FakeNtpClient::set_reply("a", a);
        FakeNtpClient::set_reply("b", b);
        FakeNtpClient::set_reply("c", c);

        NtpPoolConfig cfg;
        cfg.sample_servers = 3;
        cfg.min_valid_samples = 3;
        cfg.aggregation = NtpPoolConfig::Aggregation::Median;
        cfg.smoothing_alpha = 1.0;
        cfg.rng_seed = 1;

        Pool pool(cfg);
        std::vector<NtpServerConfig> servers;
        NtpServerConfig sa;
        sa.host = "a";
        sa.port = 123;
        servers.push_back(sa);
        NtpServerConfig sb;
        sb.host = "b";
        sb.port = 123;
        servers.push_back(sb);
        NtpServerConfig sc;
        sc.host = "c";
        sc.port = 123;
        servers.push_back(sc);
        pool.set_servers(std::move(servers));

        assert(pool.measure());
        assert(pool.offset_us() == 200);
    }

    {
        FakeNtpClient::clear();
        FakeReply x;
        x.throws_on_query = true;
        x.error_code = 5;
        FakeNtpClient::set_reply("x", x);

        NtpPoolConfig cfg;
        cfg.sample_servers = 1;
        cfg.min_valid_samples = 1;
        cfg.aggregation = NtpPoolConfig::Aggregation::Median;
        cfg.smoothing_alpha = 1.0;

        Pool pool(cfg);
        std::vector<NtpServerConfig> servers;
        NtpServerConfig sx;
        sx.host = "x";
        sx.port = 123;
        servers.push_back(sx);
        pool.set_servers(std::move(servers));

        assert(!pool.measure_n(1));
        const std::vector<NtpSample> samples = pool.last_samples();
        assert(!samples.empty());
        assert(!samples.front().is_ok);
        assert(samples.front().error_code != 0);
    }

    return 0;
}

#else
int main() {
    return 0;
}
#endif
