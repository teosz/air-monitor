var mongoose = require("mongoose");

module.exports = function(){
	var entriesSchema = mongoose.Schema({
		value: {
			type: Number,
			required: true,
		},
		date: {
			type: Date, dafault: Date.now
		}
	});
	return mongoose.model('entry', entriesSchema);
}
