#pragma once
class IRenderer
{
public:
	virtual void Render() = 0;
	virtual void PreRender() = 0;
};

