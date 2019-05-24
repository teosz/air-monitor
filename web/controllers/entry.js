var nodemailer = require('nodemailer');

module.exports = function(app) {
	var Voc = 0.3;
	const K = 0.5;
	var entry = app.models.entries;
	function createSearchForDay(day) {
		var start = new Date();
		start.setHours(0,0,0,0);
		var currentDay = start.getDay();
		var distance = currentDay - day;
		start.setDate(start.getDate() + distance);
		var end = new Date(start);
		end.setHours(23,59,59,999);
		end.setMinutes(59);
		return [
			{
				$match: {
					date: {
						$lt: end,
						$gte: start
					}
				}
			},
			{
				$group: {
					_id: null,
					avg: { $avg: "$value" }
				}
			}
		];
	}
	function createSearchForAllDays(day) {
		return [
			{
				$project: {
					dayOfWeek: { $dayOfWeek: "$date" },
					value: 1
				}
			},
			{
				$match: {
					dayOfWeek: {$eq: day},
				}
			},
			{
				$group: {
					_id: null,
					avg: { $avg: "$value" }
				}
			}
		];
	}
	var HomeController = {
		update: function(req, res){
			var model = new entry();
			var vo = parseInt(req.query.value);
			model.date = Date.now();
			vo = vo / 1024.0 * 5.0;
			dV = vo - Voc;
			if(dV < 0) {
		    dV = 0;
		    Voc = vo;
		  }
			model.value = dV / K * 10.0;
			model.save();
			res.json({success: 1});
		},
		days: function(req, res){
			queries = {};
			for(i=1;i<=7;i++) {
				queries[`avg${i}`] = createSearchForDay(i);
			}
			for(i=1;i<=7;i++) {
				queries[`all${i}`] = createSearchForAllDays(i);
			}
    	search = [{
        $facet: queries
			}];
			console.log(search);
			entry.aggregate(search, (function(error, entries) {
    		if (error) throw error;
				res.json(entries);
			}));
		},
		months: function(req, res){
			search = [
				{
					$group: {
						_id: {month: { $month: "$date" }},
						avg: { $avg: "$value" }
					}
				}
			];
			entry.aggregate(search, (function(error, entries) {
    		if (error) throw error;
				res.json(entries);
			}));
		},

		info: function(req, res){
			entry.find().sort({date: -1}).limit(1).exec(function(error, entries) {
    		if (error) throw error;
				entry.countDocuments({}, function(err, count){
					 res.json({
						 lastValue: entries[0].value,
						 date: entries[0].date,
						 count: count,
					 });
				});
			});
		},
	};

	return HomeController;
}
