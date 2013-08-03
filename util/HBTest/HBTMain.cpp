#include <gtest/gtest.h>

#include <wx/wx.h>
#include "BProject.h"
#include "wxFreeImage.h"

namespace HBTest {

	class ButcherProjectTest : public testing::Test {
	protected:
		virtual void SetUp()
		{
			Test::SetUp();
			project_.New();
		}

		// virtual void TearDown() {}

		ButcherProject project_;
	};

	class ButcherProjectViewTest : public ButcherProjectTest {
	protected:
		virtual void SetUp()
		{
			ButcherProjectTest::SetUp();

			// Add a generated image
			wxFreeImage img;
			img.New(400, 400, 24);
			wxMemoryOutputStream filedata(NULL, 0);
			img.Save(FIF_PNG, filedata, 0);
			filedata.SeekO(0);
			wxMemoryInputStream filei(filedata);

			fileid_=project_.Files().Add(wxT("TEST IMAGE"), filei);
			//project_.Files()[fileid_]->SetFileData(filedata);

			// Mask
			maskid_=project_.Masks().Add(wxT("TEST MASK"), 400, 400);

			// View
			viewid_=project_.Views().Add(wxT("TEST VIEW"), fileid_, maskid_);
		}
		// virtual void TearDown() {}

		BLID_t fileid_, maskid_, viewid_;
	};


	TEST_F(ButcherProjectTest, IsNewEmpty) {
		EXPECT_EQ(0, project_.Files().Count());
		EXPECT_EQ(0, project_.Masks().Count());
		EXPECT_EQ(0, project_.Views().Count());
	}

	TEST_F(ButcherProjectViewTest, Lines) {
		EXPECT_EQ(3, project_.Files().Count() + project_.Masks().Count() + project_.Views().Count());
	}

} // HBTest namespace

int main(int argc, char **argv) {
	wxInitializer initializer;
	if ( !initializer )
	{
		fprintf(stderr, "Failed to initialize the wxWidgets library, aborting.");

		return -1;
	}
	// Console app does not initializes GUI classes
	wxInitializeStockLists();

	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
