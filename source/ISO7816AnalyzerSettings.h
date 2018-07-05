#ifndef ISO7816_ANALYZER_SETTINGS
#define ISO7816_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class ISO7816AnalyzerSettings : public AnalyzerSettings
{
public:
	ISO7816AnalyzerSettings();
	virtual ~ISO7816AnalyzerSettings();

	virtual bool SetSettingsFromInterfaces();
	void UpdateInterfacesFromSettings();
	virtual void LoadSettings( const char* settings );
	virtual const char* SaveSettings();

	
	Channel mInputChannel;
	U32 mBitRate;
	U64 mStartTime;
	U64 mEndTime;

protected:
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mInputChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceInteger >	mBitRateInterface;
	std::auto_ptr< AnalyzerSettingInterfaceInteger >	mStartTimeInterface;
	std::auto_ptr< AnalyzerSettingInterfaceInteger >	mEndTimeInterface;
};

#endif //ISO7816_ANALYZER_SETTINGS
