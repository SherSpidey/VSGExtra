//
// Created by ParadoxFang on 2025/5/26.

#ifndef VSGAPPLICATION_H
#define VSGAPPLICATION_H

/**
 * Wrapper for vsg standard rendering process
 */
class VSGApplication {
public:
    VSGApplication();

    class VSGAppPImpl;
    
    int RenderLoop() const;

    void OpenModel(const char*) const;

private:
    VSGAppPImpl* _m_pimpl;
};



#endif //VSGAPPLICATION_H
