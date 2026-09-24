#pragma once
#include <chrono>
#include <string>
#include <map>
#include <algorithm>
#undef min
#undef max
class PerformanceProfiler {
public:
    struct SectionStats {
        float totalTime = 0.0f;
        float minTime = 999999.0f;
        float maxTime = 0.0f;
        int count = 0;

        float GetAverage() const { return count > 0 ? totalTime / count : 0.0f; }
    };

    void BeginSection(const std::string& name) {
        sectionStarts[name] = std::chrono::high_resolution_clock::now();
    }

    void EndSection(const std::string& name) {
        auto endTime = std::chrono::high_resolution_clock::now();
        auto it = sectionStarts.find(name);
        if (it != sectionStarts.end()) {
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - it->second);
            float ms = duration.count() / 1000.0f;

            auto& stats = sectionStats[name];
            stats.totalTime += ms;
            stats.minTime = std::min(stats.minTime, ms);
            stats.maxTime = std::max(stats.maxTime, ms);
            stats.count++;
        }
    }

    void PrintStats() {
        printf("\n╔════════════════════════════════════════════════════════════════╗\n");
        printf("║               PERFORMANCE PROFILE REPORT                       ║\n");
        printf("╚════════════════════════════════════════════════════════════════╝\n\n");

        float totalFrameTime = 0.0f;

        for (const auto& pair : sectionStats) {
            const auto& stats = pair.second;
            float avg = stats.GetAverage();
            totalFrameTime += avg;

            printf("%-25s: Avg: %6.2fms  Min: %6.2fms  Max: %6.2fms\n",
                pair.first.c_str(), avg, stats.minTime, stats.maxTime);

            // Warnings
            if (avg > 5.0f) {
                printf("  ⚠️  WARNING: High average time (>5ms) - MAJOR BOTTLENECK!\n");
            }
            else if (avg > 2.0f) {
                printf("  ⚠️  Moderate time (>2ms) - optimization recommended\n");
            }

            if (stats.maxTime > 20.0f) {
                printf("  ⚠️  Large spikes detected (%.2fms max) - stuttering!\n", stats.maxTime);
            }
        }

        printf("\n─────────────────────────────────────────────────────────────────\n");
        printf("Total Frame Time: %.2fms (%.0f FPS)\n", totalFrameTime, 1000.0f / totalFrameTime);
        printf("═════════════════════════════════════════════════════════════════\n\n");

        // Recommendations
        if (totalFrameTime > 16.67f) {
            printf("🔴 CRITICAL: Frame time exceeds 16.67ms (below 60 FPS)\n\n");
        }
        else if (totalFrameTime > 6.94f) {
            printf("⚠️  Frame time exceeds 6.94ms (below 144 FPS)\n\n");
        }
        else {
            printf("✅ Performance is good (>144 FPS)\n\n");
        }
    }

    void Reset() {
        sectionStats.clear();
    }

private:
    std::map<std::string, std::chrono::high_resolution_clock::time_point> sectionStarts;
    std::map<std::string, SectionStats> sectionStats;
};