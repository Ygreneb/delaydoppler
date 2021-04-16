#pragma once

#include "PathEdit.h"

#include "vstgui/lib/iviewlistener.h"
#include "vstgui/uidescription/icontroller.h"
#include "vstgui/uidescription/delegationcontroller.h"

namespace Steinberg {
namespace DelayDoppler {

class PlugController;


class PathSubcontroller : public VSTGUI::IController/*, public VSTGUI::ViewListenerAdapter*/
{
	using CControl = VSTGUI::CControl;
	using CTextButton = VSTGUI::CTextButton;
	using CView = VSTGUI::CView;
	using UIAttributes = VSTGUI::UIAttributes;
	using IUIDescription = VSTGUI::IUIDescription;
	using PathEdit = VSTGUI::PathEdit;

	enum Tags
	{
		kPathEditTag = 666
	};

public:
	PathSubcontroller(PlugController* mainController)
		: mMainController(mainController)
		, mPathEdit(nullptr)
		//, mAddPointBtn(nullptr)
		//, mRemovePointBtn(nullptr)
	{}

	~PathSubcontroller()
	{
		if (mPathEdit)
		{
			//mPathEdit->unregisterControlListener(this);
			mPathEdit->forget();
			mPathEdit = nullptr;
		}
		//if (mAddPointBtn)
		//{
		//	mAddPointBtn->unregisterControlListener(this);
		//	mAddPointBtn->forget();
		//	mAddPointBtn = nullptr;
		//}
		//if (mRemovePointBtn)
		//{
		//	mRemovePointBtn->unregisterControlListener(this);
		//	mRemovePointBtn->forget();
		//	mRemovePointBtn = nullptr;
		//}
		mMainController->removePathSubcontroller(this);
	}

	// ------ from IController ------
	CView* verifyView(CView* view, const UIAttributes& attributes, const IUIDescription* description) override
	{
		if (PathEdit* pc = dynamic_cast<PathEdit*> (view))
		{
			mPathEdit = pc;
			//mPathEdit->registerControlListener(this); // is already the main listener, but why?!
			mPathEdit->remember();

			Point points[kMaxNumPoints];
			int32 numPoints;
			mMainController->getPoints(points, numPoints);
			mPathEdit->setPoints(points, numPoints);
		}
		//if (CTextButton* btn = dynamic_cast<CTextButton*>(view))
		//{
		//	if (btn->getTag() == kAddPointId)
		//	{
		//		mAddPointBtn = btn;
		//		mAddPointBtn->registerControlListener(this);
		//		mAddPointBtn->remember();
		//	}
		//	else if (btn->getTag() == kRemovePointId)
		//	{
		//		mRemovePointBtn = btn;
		//		mRemovePointBtn->registerControlListener(this);
		//		mRemovePointBtn->remember();
		//	}
		//}
		return view;
	}

	void controlEndEdit(CControl* pControl) override
	{
		// Buttons only, dragging control points is done in valueChanged()
		if (pControl->getTag() == kAddPointId)
		//if (mAddPointBtn && pControl == mAddPointBtn)
		{
			mPathEdit->addPoint();
		}
		else if (pControl->getTag() == kRemovePointId)
		{
			mPathEdit->removePoint();
		}
	}

	//void viewWillDelete(CView* view) override
	//{
	//	if (dynamic_cast<PathEdit*> (view) == mPathEdit)
	//	{
	//		//mPathEdit->unregisterViewListener(this);
	//		mPathEdit->unregisterControlListener(this);
	//		mPathEdit->forget();
	//		mPathEdit = nullptr;
	//	}
	//}

	void valueChanged(CControl* pControl) override
	{
		//if (pControl->getTag() == kPathEditTag)
		if (mPathEdit && dynamic_cast<PathEdit*>(pControl) == mPathEdit)
		{
			//PathEdit* PathEdit = dynamic_cast<PathEdit*>(pControl);
			//if (PathEdit)
			//{
			Point* points = new Point[kMaxNumPoints];
			uint32_t numpoints = kMaxNumPoints;
			mPathEdit->getPoints(points, numpoints);

			if (IPtr<Vst::IMessage> message = owned(mMainController->allocateMessage()))
			{
				message->setMessageID("BinaryMessage");
				message->getAttributes()->setBinary("PointData", points, sizeof(Point) * numpoints);
				mMainController->sendMessage(message);
				mMainController->performEdit(kPathPointChange, 0.0); // This hopefully triggers informs host that path has changed and makes it ask for saving before closing
			}

			delete[] points;
			//}
		}
	}

	// TODO use STL vector
	//void updatePathPoints(Point* points, uint32 num)
	//{
	//	if (mPathEdit)
	//	{
	//		mPathEdit->setPoints(points, num);
	//	}
	//}

	//void sendPointDataToPathEdit(Point a, Point b)
	//{
	//	if (mPathEdit)
	//	{
	//		Point p[] = { a,b };
	//		mPathEdit->setPoints(p, 2);
	//	}
	//}

	inline void setProgress(double p)
	{
		if (mPathEdit)
		{
			mPathEdit->setProgress(p);
		}
	}

	inline void setLoop(bool l)
	{
		if (mPathEdit)
		{
			mPathEdit->setLoop(l);
		}
	}

	//void controlEndEdit(CControl* pControl) override
	//{
	//	
	//}

private:
	PlugController* mMainController;
	PathEdit* mPathEdit;
	//CTextButton* mAddPointBtn, * mRemovePointBtn;

	////--- is called when a view is created -----
	//CView* verifyView(CView* view, const UIAttributes& /*attributes*/,
	//	const IUIDescription* /*description*/) override
	//{
	//	if (CTextEdit* te = dynamic_cast<CTextEdit*> (view))
	//	{
	//		// this allows us to keep a pointer of the text edit view
	//		textEdit = te;

	//		// add this as listener in order to get viewWillDelete and viewLostFocus calls
	//		textEdit->registerViewListener(this);

	//		// initialize it content
	//		String str(againController->getDefaultMessageText());
	//		str.toMultiByte(kCP_Utf8);
	//		textEdit->setText(str.text8());
	//	}
	//	return view;
	//}
	////--- from IViewListenerAdapter ----------------------
	////--- is called when a view will be deleted: the editor is closed -----
	//void viewWillDelete(CView* view) override
	//{
	//	if (dynamic_cast<CTextEdit*> (view) == textEdit)
	//	{
	//		textEdit->unregisterViewListener(this);
	//		textEdit = nullptr;
	//	}
	//}
	////--- is called when the view is loosing the focus -----------------
	//void viewLostFocus(CView* view) override
	//{
	//	if (dynamic_cast<CTextEdit*> (view) == textEdit)
	//	{
	//		// save the last content of the text edit view
	//		const UTF8String& text = textEdit->getText();
	//		String128 messageText;
	//		String str;
	//		str.fromUTF8(text.data());
	//		str.copyTo(messageText, 0, 128);
	//		againController->setDefaultMessageText(messageText);
	//	}
	//}
};

}
}