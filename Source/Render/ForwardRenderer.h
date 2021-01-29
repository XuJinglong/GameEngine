#pragma once
#include "EngineCommon.h"
#include "IRenderer.h"

class ForwardRender : public IRenderer
{
public:
	ForwardRender() {}
	~ForwardRender(){}

	virtual void PreRender() override;
	virtual void Render() override;
};

