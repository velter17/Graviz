/**
 * Project   AlgoVi
 *
 * @file     CVisualizationController.cpp
 * @author   Dmytro Sadovyi
 * @date     16.12.2016
 */

#include "../CVisualizationController.h"
#include "View/components/CPoint.h"

#include "View/problems/CConvexHullViz.h"
#include "View/problems/CCppScriptViz.h"
#include "View/problems/CDistToCurveViz.h"
#include "View/problems/CSegmentCrossViz.h"

namespace NView
{

CVisualizationController::CVisualizationController(
        std::shared_ptr<CGraphicView> view,
        std::shared_ptr<NCommand::CCompilerHandler> compilerHandler,
        NCommand::CVisualSolver *visualSolver)
    : mView(view)
    , mVisualSolver(visualSolver)
    , mCompilerHandler(compilerHandler)
    , mMode(1)
    , mCurrentPrefix("Point")
    , mSolverMode(0)
{
    mConnections.push_back(connect(mView.get(), &CGraphicView::objectAdded, [this](IAlgoViItem* item){
        mObjectsMap[item->getName()] = item;
    }));
    mConnections.push_back(connect(mView.get(), &CGraphicView::objectChangeName, [this](IAlgoViItem* item, QString oldName){
        mObjectsMap.erase(oldName);
        mObjectsMap[item->getName()] = item;
    }));
    mConnections.push_back(connect(mView.get(), &CGraphicView::changed, [this](const QList<QRectF>& region){
        update();
    }));
    mConnections.push_back(connect(mView.get(), SIGNAL(mousePressed(QPoint)), this, SLOT(addPoint(QPoint))));

    initVisualizer();

    mView->setVisualizer(mVisualizer);
    mView->setObjectsMap(&mObjectsMap);
}

CVisualizationController::~CVisualizationController()
{
    mView->clear();
    mView->setDataToDraw("");
}

void CVisualizationController::finish()
{
    for(QMetaObject::Connection& c : mConnections)
        disconnect(c);
}

void CVisualizationController::addPoint(const QPoint &p)
{
    if(mMode == 1)
    {
        qDebug () << "add " << mCurrentPrefix << " in pos " << p;
        QString name = mCurrentPrefix + "#" + QString::number(++mPointNum[mCurrentPrefix]);
        mView->addAlgoViItem(new CPoint(name, p));
    }
}

void CVisualizationController::updateResult(const QString &data)
{
    mView->setDataToDraw(data);
}

void CVisualizationController::handleInput(const QString &data)
{
    qDebug () << "CVisualizerController> handleInput: " << data;
    QStringList args = data.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    if(args.isEmpty())
        return;
    QString command = *args.begin();
    args.erase(args.begin());
    if(command == "add")
    {
        if(args.isEmpty())
            return;
        if(*args.begin() == "on")
            mMode = 1;
        else
            mMode = 0;
    }
    if(command == "set-prefix")
    {
        if(args.isEmpty())
            return;
        mCurrentPrefix = *args.begin();
    }
    if(command == "mode")
    {
       if(args.isEmpty())
          return;
       if(*args.begin() == "1")
          mSolverMode = 1;
       else
          mSolverMode = 0;
    }
}

void CVisualizationController::update()
{
    if(mVisualizer->isFree() && mSolverMode)
        emit sceneChanged(mVisualizer->serialize(mObjectsMap));
}

void CVisualizationController::initVisualizer()
{
    QString algoPath = mVisualSolver->getAlgoScriptPath();
    qDebug () << "algoPath = " << algoPath;
    if(algoPath == "convex_hull")
        mVisualizer.reset(new CConvexHullViz);
    else if(algoPath == "route")
        mVisualizer.reset(new CSegmentCrossViz);
    else if(algoPath == "dist_to_curve")
        mVisualizer.reset(new CDistToCurveViz);
    else
        mVisualizer.reset(new CCppScriptViz(mCompilerHandler, algoPath));
}


} // namespace NView
