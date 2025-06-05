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
    
    int RenderLoop() const;

    void OpenModel(const char*) const;

private:
    class VSGAppPImpl;
    VSGAppPImpl* _m_pimpl;
};



#endif //VSGAPPLICATION_H
