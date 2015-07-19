#include <iostream>

#include <QtCore/QString.h>
#include <support/helper/hexdump.h>
#include <support_qt/helper/gui_helper.h>
#include <support_qt/logging/logging_item_model.h>

#include <plugins/plugin_manager.h>

#include <manipulator/editor/manipulator_editor_dlg_gui.moc>

int main(int argc, char *argv[])
{
	QString s = "Test";
	supportlib::string::string_t out;

	std::vector<char> l;
	l.push_back('a');
	l.push_back('b');
	out = toHexArray(l);

	LoggingItemModel dlg;
	dlg.clear();
	PluginManager plg;
	plg.clearPaths();
	QList<IManipulator *> m;
	QList<DatabaseColumn *> col;
	ManipulatorEditorDialogBox mbox(m, col, NULL);
	mbox.getManipulators();

	std::cout << argc << ":" << argv[0] << " ... " << out << std::endl;
	std::cout << "button:" << (void *)supportlib::image::button_add << std::endl;
	return 0;
}
