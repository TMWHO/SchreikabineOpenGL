#pragma once

#include <JuceHeader.h>
#include "AudioState.h"
#include <deque>

class OpenGLScopeView final : public juce::Component, private juce::OpenGLRenderer
{
public:
    explicit OpenGLScopeView(AudioState& state);
    ~OpenGLScopeView() override;

    void setRenderData(const std::vector<float>& values);
    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    struct LineVertex { float x; float y; float z; };
    struct ParticleVertex { float x; float y; float z; float alpha; };
    struct Particle { float x; float y; float vy; float alpha; float age; };
    struct SpectrumFrame { std::vector<float> values; double timeMs; };

    static const char* const vertexShader;
    static const char* const fragmentShader;

    void newOpenGLContextCreated() override;
    void openGLContextClosing() override;
    void renderOpenGL() override;

    void copyPendingData();
    void syncVisualParams();
    float getCurrentSpectrumPeak() const;

    void updateSpectrumVerticesAndSpawn(int width, int height, float scale);

    void spawnParticlesFromSpectrum(int width, int height);

    void updateParticles(int height, float scale);

    void buildParticleVertices();

    void buildMeshVertices();

    static float frequencyToX(float freq, float width);

    void uploadBuffer(GLuint buffer, const std::vector<LineVertex>& vertices);
    void uploadBuffer(GLuint buffer, const std::vector<ParticleVertex>& vertices);

    void drawLine(float red, float green, float blue, float alpha, float lineWidth);

    void drawGlowPoints(float scale);

    void drawLiveSpectrumPoints(float scale);

    void drawParticleDebugAnchors(float scale);

    void drawLineBuffer(GLuint buffer, GLenum primitive, int count);

    void drawBuffer(GLuint buffer, GLenum primitive, int count);

    AudioState& audioState;
    juce::OpenGLContext openGLContext;
    std::unique_ptr<juce::OpenGLShaderProgram> shader;
    std::unique_ptr<juce::OpenGLShaderProgram::Attribute> positionAttribute;
    std::unique_ptr<juce::OpenGLShaderProgram::Attribute> alphaAttribute;
    GLuint lineBuffer{0};
    GLuint particleBuffer{0};
    GLuint meshBuffer{0};
    GLuint axisBuffer{0};
    juce::CriticalSection dataLock;
    std::vector<float> pendingSpectrum;
    std::vector<float> currentSpectrum;
    std::vector<LineVertex> spectrumVertices;
    std::vector<ParticleVertex> particleVertices;
    std::vector<ParticleVertex> meshVertices;
    std::vector<LineVertex> axisVertices;
    std::vector<Particle> particles;
    std::deque<SpectrumFrame> spectrumHistory;
    juce::Random particleRandom;
    bool spectrumDirty{false};
    double lastFrameTimeMs{0.0};
    float alphaGlow{0.7f};
    float particleRadius{1.0f};
    float spectrumFloorNdc{-0.85f};
    float spectrumTopNdc{0.98f};
    static constexpr float axisOffsetPixels = 34.0f;
    static constexpr float axisPlotGapPixels = 6.0f;
    static constexpr float viewZoom = 0.92f;
    static constexpr float viewOffsetX = 0.0f;
    static constexpr float viewOffsetY = -0.10f;
    static constexpr size_t maxHistoryFrames = 240;
    bool hasPendingData{false};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OpenGLScopeView)
};
