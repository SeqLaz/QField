$(function () {
  "use strict";

  /* ==========================================================================
       yearly payments request
     ========================================================================== */
  const MONTHLY_USER = 16;
  const MONTHLY_GB = 5;

  const users = $("#users");
  const storage = $("#storage");
  const costs = $("#costs");
  const iframe = $("#request-form");

  function updateCosts() {
    const users_val = users.val()
    const storage_val = storage.val()
    
    const total = (Math.max(users_val, 2) * MONTHLY_USER + Math.max((storage_val - 1), 0) * MONTHLY_GB) * 12;
    costs.val(total);
    updateForm(users_val, storage_val)
  }

  function updateForm(users, storage){
    const params = `&Total%20GB%20needed=${storage}&Number%20of%20collaborators=${users}`
    const form_url = "https://forms.clickup.com/2192114/f/22wqj-21867/FV88S70BQG7ERZ65R9?monthly_gb_price=5&monthly_user_price=16"
    // &Total%20GB%20needed=10&Number%20of%20collaborators=4
    const url = form_url + params
    iframe.attr('src', url);
    console.log(url);

  }

  users.on('input', function () {
    updateCosts();
  });

  storage.on('input', function () {
    updateCosts();
  });
});
