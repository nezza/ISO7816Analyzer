#ifndef ISO7816_ANALYZER_H
#define ISO7816_ANALYZER_H

#include <Analyzer.h>
#include "ISO7816AnalyzerResults.h"
#include "SimpleSerialSimulationDataGenerator.h"

class ISO7816AnalyzerSettings;
class ANALYZER_EXPORT ISO7816Analyzer : public Analyzer2
{
public:
	ISO7816Analyzer();
	virtual ~ISO7816Analyzer();

	virtual void SetupResults();
	virtual void WorkerThread();

	virtual U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels );
	virtual U32 GetMinimumSampleRateHz();

	virtual const char* GetAnalyzerName() const;
	virtual bool NeedsRerun();

protected: //vars
	std::auto_ptr< ISO7816AnalyzerSettings > mSettings;
	std::auto_ptr< ISO7816AnalyzerResults > mResults;
	AnalyzerChannelData* mSerial;

	SimpleSerialSimulationDataGenerator mSimulationDataGenerator;
	bool mSimulationInitilized;

	//Serial analysis vars:
	U32 mSampleRateHz;
	U32 mStartOfStopBitOffset;
	U32 mEndOfStopBitOffset;
};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer( );
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer( Analyzer* analyzer );

#endif //ISO7816_ANALYZER_H
