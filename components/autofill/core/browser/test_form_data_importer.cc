// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/autofill/core/browser/test_form_data_importer.h"

namespace autofill {

TestFormDataImporter::TestFormDataImporter(
    AutofillClient* client,
    payments::PaymentsClient* payments_client,
    std::unique_ptr<CreditCardSaveManager> credit_card_save_manager,
    PersonalDataManager* personal_data_manager,
    const std::string& app_locale)
    : FormDataImporter(client,
                       payments_client,
                       personal_data_manager,
                       app_locale) {
  set_credit_card_save_manager(std::move(credit_card_save_manager));
}

}  // namespace autofill
