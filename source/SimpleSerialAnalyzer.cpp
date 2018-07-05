#include "SimpleSerialAnalyzer.h"
#include "SimpleSerialAnalyzerSettings.h"
#include <AnalyzerChannelData.h>

#include <iostream>
SimpleSerialAnalyzer::SimpleSerialAnalyzer()
:	Analyzer2(),  
	mSettings( new SimpleSerialAnalyzerSettings() ),
	mSimulationInitilized( false )
{
	SetAnalyzerSettings( mSettings.get() );
}

SimpleSerialAnalyzer::~SimpleSerialAnalyzer()
{
	KillThread();
}

void SimpleSerialAnalyzer::SetupResults()
{
	mResults.reset( new SimpleSerialAnalyzerResults( this, mSettings.get() ) );
	SetAnalyzerResults( mResults.get() );
	mResults->AddChannelBubblesWillAppearOn( mSettings->mInputChannel );
}

void SimpleSerialAnalyzer::WorkerThread()
{
	mSampleRateHz = GetSampleRate();
	U64 start_sample = (mSampleRateHz/1000) * mSettings->mStartTime;
	U64 end_sample = (mSampleRateHz/1000) * mSettings->mEndTime;


	mSerial = GetAnalyzerChannelData( mSettings->mInputChannel );

	if( start_sample )
		mSerial->Advance(start_sample);

	// Find first rising edge
	if( mSerial->GetBitState() == BIT_LOW )
		mSerial->AdvanceToNextEdge();


	double smallest_period = 1.0/mSettings->mBitRate;

	U32 samples_per_bit = mSampleRateHz / (1.0/smallest_period);

	U32 samples_half = samples_per_bit/2;


	for( ; ; )
	{
		U8 data = 0;
		U8 mask = 1;
		

		mSerial->AdvanceToNextEdge(); //falling edge -- beginning of the start bit

		if(end_sample && mSerial->GetSampleNumber() > end_sample)
			continue;

		mSerial->Advance(samples_half);

		// add dot on start bit
		mResults->AddMarker( mSerial->GetSampleNumber(), AnalyzerResults::Dot, mSettings->mInputChannel );


		U64 starting_sample = mSerial->GetSampleNumber();

		mSerial->Advance( samples_per_bit );

		for( U32 i=0; i<8; i++ )
		{
			//let's put a dot exactly where we sample this bit:
			mResults->AddMarker( mSerial->GetSampleNumber(), AnalyzerResults::Dot, mSettings->mInputChannel );

			if( mSerial->GetBitState() == BIT_HIGH )
				data |= mask;

			mSerial->Advance( samples_per_bit );

			mask = mask << 1;
		}


		// add dot on stop bit
		mResults->AddMarker( mSerial->GetSampleNumber(), AnalyzerResults::Dot, mSettings->mInputChannel );

		// Advance over stopbit
		mSerial->Advance(samples_per_bit);



		//we have a byte to save. 
		Frame frame;
		frame.mData1 = data;
		frame.mFlags = 0;
		frame.mStartingSampleInclusive = starting_sample;
		frame.mEndingSampleInclusive = mSerial->GetSampleNumber();

		mResults->AddFrame( frame );
		mResults->CommitResults();
		ReportProgress( frame.mEndingSampleInclusive );
	}
}

bool SimpleSerialAnalyzer::NeedsRerun()
{
	return false;
}

U32 SimpleSerialAnalyzer::GenerateSimulationData( U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor** simulation_channels )
{
	if( mSimulationInitilized == false )
	{
		mSimulationDataGenerator.Initialize( GetSimulationSampleRate(), mSettings.get() );
		mSimulationInitilized = true;
	}

	return mSimulationDataGenerator.GenerateSimulationData( minimum_sample_index, device_sample_rate, simulation_channels );
}

U32 SimpleSerialAnalyzer::GetMinimumSampleRateHz()
{
	return mSettings->mBitRate * 4;
}

const char* SimpleSerialAnalyzer::GetAnalyzerName() const
{
	return "ISO7816";
}

const char* GetAnalyzerName()
{
	return "ISO7816";
}

Analyzer* CreateAnalyzer()
{
	return new SimpleSerialAnalyzer();
}

void DestroyAnalyzer( Analyzer* analyzer )
{
	delete analyzer;
}