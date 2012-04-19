#ifndef COMPONENT_H
#define COMPONENT_H

namespace icinga
{

class I2_BASE_API Component : public Object
{
private:
	Application::WeakPtr m_Application;
	ConfigObject::Ptr m_Config;

public:
	typedef shared_ptr<Component> Ptr;
	typedef weak_ptr<Component> WeakPtr;

	void SetApplication(const Application::WeakPtr& application);
	Application::Ptr GetApplication(void) const;

	void SetConfig(const ConfigObject::Ptr& componentConfig);
	ConfigObject::Ptr GetConfig(void) const;

	virtual string GetName(void) const = 0;
	virtual void Start(void) = 0;
	virtual void Stop(void) = 0;
};

#define EXPORT_COMPONENT(klass) \
	extern "C" I2_EXPORT icinga::Component *CreateComponent(void)	\
	{														\
		return new klass();									\
	}

}

#endif /* COMPONENT_H */
