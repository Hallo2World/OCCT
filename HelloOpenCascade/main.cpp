#include <BRepPrimAPI_MakeBox.hxx>
#include <IVtkTools_ShapeDataSource.hxx>

#include <vtkAutoInit.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyDataMapper.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)

int main(int argc, char* argv[])
{
	BRepPrimAPI_MakeBox mkBox(1., 2., 3);
	const TopoDS_Shape& shape = mkBox.Shape();

	vtkNew<vtkRenderWindow> renwin;
	vtkNew<vtkRenderer> ren;

	renwin->AddRenderer(ren);

	vtkNew<vtkInteractorStyleTrackballCamera> istyle;
	vtkNew<vtkRenderWindowInteractor> iren;

	iren->SetRenderWindow(renwin);
	iren->SetInteractorStyle(istyle);

	// Domain data(TopoDS_Shape) -> vtkPolyData -> filter - > filter -> ... -> mapper -> actor
	// Domain data(TopoDS_Shape) -> vtkPolyData -> www. -> mapper -> actor

	vtkNew<IVtkTools_ShapeDataSource> occSource;
	occSource->SetShape(new IVtkOCC_Shape(shape));
		 
	vtkNew<vtkPolyDataMapper> mapper;
	mapper->SetInputConnection(occSource->GetOutputPort());

	vtkNew<vtkActor> actor;
	actor->SetMapper(mapper);
	ren->AddActor(actor);

	renwin->Render();
	iren->Start();

	return 0;
}