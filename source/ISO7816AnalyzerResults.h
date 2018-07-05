#ifndef ISO7816_ANALYZER_RESULTS
#define ISO7816_ANALYZER_RESULTS

#include <AnalyzerResults.h>

class ISO7816Analyzer;
class ISO7816AnalyzerSettings;

class ISO7816AnalyzerResults : public AnalyzerResults
{
public:
	ISO7816AnalyzerResults( ISO7816Analyzer* analyzer, ISO7816AnalyzerSettings* settings );
	virtual ~ISO7816AnalyzerResults();

	virtual void GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base );
	virtual void GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id );

	virtual void GenerateFrameTabularText(U64 frame_index, DisplayBase display_base );
	virtual void GeneratePacketTabularText( U64 packet_id, DisplayBase display_base );
	virtual void GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base );

protected: //functions

protected:  //vars
	ISO7816AnalyzerSettings* mSettings;
	ISO7816Analyzer* mAnalyzer;
};

#endif //ISO7816_ANALYZER_RESULTS
