#include "ISO7816AnalyzerSettings.h"
#include <AnalyzerHelpers.h>


ISO7816AnalyzerSettings::ISO7816AnalyzerSettings()
:	mInputChannel( UNDEFINED_CHANNEL ),
	mBitRate( 9600 ),
	mStartTime( 0 ),
	mEndTime( 0 )
{
	mInputChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mInputChannelInterface->SetTitleAndTooltip( "ISO7816", "Simple ISO7816 decoder." );
	mInputChannelInterface->SetChannel( mInputChannel );

	mBitRateInterface.reset( new AnalyzerSettingInterfaceInteger() );
	mBitRateInterface->SetTitleAndTooltip( "Bit Rate (Bits/S)",  "Specify the bit rate in bits per second." );
	mBitRateInterface->SetMax( 6000000 );
	mBitRateInterface->SetMin( 1 );
	mBitRateInterface->SetInteger( mBitRate );


	mStartTimeInterface.reset(new AnalyzerSettingInterfaceInteger() );
	mStartTimeInterface->SetTitleAndTooltip( "Start time (ms)",  "Specify where the analyzer should start." );
	mStartTimeInterface->SetMax( 10000000 );
	mStartTimeInterface->SetMin( 0 );
	mStartTimeInterface->SetInteger( mBitRate );
	mEndTimeInterface.reset(new AnalyzerSettingInterfaceInteger() );
	mEndTimeInterface->SetTitleAndTooltip( "End time (ms)",  "Specify where the analyzer should stop." );
	mEndTimeInterface->SetMax( 10000000 );
	mEndTimeInterface->SetMin( 0 );
	mEndTimeInterface->SetInteger( mBitRate );


	AddInterface( mInputChannelInterface.get() );
	AddInterface( mBitRateInterface.get() );
	AddInterface( mStartTimeInterface.get() );
	AddInterface( mEndTimeInterface.get() );

	AddExportOption( 0, "Export as text/csv file" );
	AddExportExtension( 0, "text", "txt" );
	AddExportExtension( 0, "csv", "csv" );

	ClearChannels();
	AddChannel( mInputChannel, "I/O", false );
}

ISO7816AnalyzerSettings::~ISO7816AnalyzerSettings()
{
}

bool ISO7816AnalyzerSettings::SetSettingsFromInterfaces()
{
	mInputChannel = mInputChannelInterface->GetChannel();
	mBitRate = mBitRateInterface->GetInteger();
	mStartTime = mStartTimeInterface->GetInteger();
	mEndTime = mEndTimeInterface->GetInteger();

	ClearChannels();
	AddChannel( mInputChannel, "ISO7816", true );

	return true;
}

void ISO7816AnalyzerSettings::UpdateInterfacesFromSettings()
{
	mInputChannelInterface->SetChannel( mInputChannel );
	mBitRateInterface->SetInteger( mBitRate );
	mStartTimeInterface->SetInteger( mStartTime );
	mEndTimeInterface->SetInteger( mEndTime );
}

void ISO7816AnalyzerSettings::LoadSettings( const char* settings )
{
	SimpleArchive text_archive;
	text_archive.SetString( settings );

	text_archive >> mInputChannel;
	text_archive >> mBitRate;
	text_archive >> mStartTime;
	text_archive >> mEndTime;

	ClearChannels();
	AddChannel( mInputChannel, "ISO7816", true );

	UpdateInterfacesFromSettings();
}

const char* ISO7816AnalyzerSettings::SaveSettings()
{
	SimpleArchive text_archive;

	text_archive << mInputChannel;
	text_archive << mBitRate;
	text_archive << mStartTime;
	text_archive << mEndTime;

	return SetReturnString( text_archive.GetString() );
}
