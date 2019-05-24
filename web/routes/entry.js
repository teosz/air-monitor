module.exports = function(app){
	var entry = app.controllers.entry;
	app.route('/update')
		.get(entry.update)
	app.route('/days')
		.get(entry.days)
	app.route('/months')
		.get(entry.months)
	app.route('/info')
		.get(entry.info)

}
